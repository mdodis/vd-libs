function makeToc(contentElement, tocSelector, options) {
  if (options == null) {
    options = {};
  }
  if (contentElement == null) {
    throw new Error('need to provide a selector where to scan for headers');
  }
  if (tocSelector == null) {
    throw new Error('need to provide a selector where inject the TOC');
  }
  // Support <section class="Lx ..."> and <div class="Lx ...">
  var elements;
  if (typeof contentElement === 'string') {
    elements = document.querySelectorAll(contentElement + ' section[class^="L"], ' + contentElement + ' div[class^="L"]');
  } else {
    elements = contentElement.querySelectorAll('section[class^="L"], div[class^="L"]');
  }
  var allSections = Array.prototype.slice.call(elements);
  var min = 6;
  var headers = allSections.filter(function(item) {
    var classesList = item.className.split(' ');
    if (classesList.indexOf("toc-ignore") != -1) {
      return false;
    }
    if ((options.ignore || []).indexOf(getText(item)) != -1) {
      return false;
    }
    // Find L[1-6] class
    var level = null;
    for (var i = 0; i < classesList.length; i++) {
      var m = /^L(\d)$/.exec(classesList[i]);
      if (m) {
        level = parseInt(m[1]);
        break;
      }
    }
    if (level && level >= 1 && level <= (options.max || 6)) {
      min = Math.min(min, level);
      item._tocLevel = level;
      return true;
    }
    return false;
  });
  var hierarchy = createHierarchy(headers, min, true);
  var toc = parseNavNodes(hierarchy.nodes, min);
  var container = document.querySelector(tocSelector);
  setText(container, '');
  container.appendChild(toc);
}

function createHierarchy(headers, minLevel, useSectionLevel) {
  var hierarchy = { nodes: [] };
  window.hierarchy = hierarchy;
  var previousNode = { parent: hierarchy };
  var level = minLevel;
  var init = false;
  headers.forEach(function(header) {
    var headingNumber = useSectionLevel ? header._tocLevel : parseInt(header.nodeName.substr(1));
    var title = '';
    if (header.tagName === 'SECTION') {
      var h4 = header.querySelector('h4');
      title = h4 ? getText(h4) : getText(header);
    } else if (header.tagName === 'DIV') {
      title = header.id ? header.id : getText(header);
    } else {
      title = getText(header);
    }
    var object = {
      title: title,
      link: window.location.pathname + (header.id ? ('#' + header.id) : ''),
      originLevel: headingNumber,
      nodes: []
    };
    if (headingNumber === level) {
      object.parent = previousNode.parent;
      // keep level
    } else if (headingNumber - level >= 1) {
      // go one step deeper, regardless how much
      if (init === false) {
        var missingParent = {
          parent: previousNode.parent,
          title: '',
          link: '',
          originLevel: NaN,
          nodes: []
        };
        previousNode.parent.nodes.push(missingParent);
        previousNode = missingParent;
      }
      object.parent = previousNode;
      level++;
    } else if (level - headingNumber >= 1) {
      // go one or more step up again
      var ref = previousNode.parent;
      while (level - headingNumber >= 1) {
        ref = ref.parent;
        level--;
      }
      object.parent = ref;
    } else {
      console.error('unknown toc path');
    }
    object.parent.nodes.push(object);
    previousNode = object;
    init = true;
  });
  return hierarchy;
}

function parseNavNodes(nodes) {
  // Create <nav> root
  var nav = document.createElement('nav');
  nav.className = 'toc-outline p-0 nav outline-section flex-column';
  var minLevel = arguments[1];
  nodes.forEach(function(node) {
    nav.appendChild(parseNavNode(node, minLevel));
  });
  return nav;
}

function parseNavNode(node, minLevel) {
  var level = node.originLevel;
  var a = document.createElement('a');
  a.className = 'nav-link';
  a.href = node.link;
  setText(a, node.title);
  if (node.nodes && node.nodes.length > 0) {
    // If has children, wrap them in a nested <div>
    var container = document.createElement('div');
    container.className = 'nav outline-section flex-column ms-3';
    node.nodes.forEach(function (child) {
      container.appendChild(parseNavNode(child, minLevel));
    });
    var frag = document.createDocumentFragment();
    frag.appendChild(a);
    frag.appendChild(container);
    return frag;
  } else {
    return a;
  }
}

function getText(elem) {
    if (elem.textContent != null) {
        return elem.textContent;
    } else {
        elem.innerText;
    }
}

function setText(elem, value) {
    if (elem.textContent != null) {
        elem.textContent = value;
    } else {
        elem.innerText = value;
    }
}

  function runToc() {

    var element = document.querySelector("[data-toc]");
    if (element != null) {
      var options = {};
      var ignore = (element.attributes.getNamedItem("data-toc-ignore") || {}).value
      var max = (element.attributes.getNamedItem("data-toc-max") || {}).value
      if (ignore != null) {
        options.ignore = ignore;
      }
      if (max != null) {
        options.max = parseInt(max);
      }
      makeToc(element.parentNode, '[data-toc]', options);
    }
  }

document.addEventListener('DOMContentLoaded', function () {
  runToc();
});
