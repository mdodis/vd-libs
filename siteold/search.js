let searchIndex = null;
let fuse = null;

fetch("/search-index.json")
    .then(response => response.json())
    .then(data => {
        const options = {
          keys: ["contents"],
          includeScore: true,
          threshold: 0.3
        };
        searchIndex = data;

        fuse = new Fuse(data, options);
        // // Example search:
        // const results = fuse.search("for how to use");
        // console.log(results);
    });

const searchBox = document.getElementById("searchbox");
const searchBoxResults = document.getElementById("search-results");

function searchBoxDebounce(fn, delay) {
    let timeout;
    return function(...args) {
        clearTimeout(timeout);
        timeout = setTimeout(() => fn.apply(this, args), delay);
    };
}

function searchBoxRenderResults(results) {
    searchBoxResults.innerHTML = "";
    if (results.length === 0) {
        searchBoxResults.style.display = "none";
        return;
    }

    results.forEach(result => {
        const li = document.createElement("li");
        li.className = "list-group-item list-group-item-action search-result";
        li.textContent = result.item.page + " - " + result.item.section;
        li.tabIndex = 0;

        li.addEventListener("click", () => {
            const url = `${result.item.page}#${result.item.section}`;
            window.location.href = url;
        })
        searchBoxResults.appendChild(li);
    });

    searchBoxResults.style.display = "block";
}

function searchBoxSearch(query) {
    console.log("Searching for " + query);
    if (!query.trim()) {
        searchBoxResults.innerHTML = "";
        searchBoxResults.style.display = "none";
        return;
    }
    const results = fuse.search(query);
    searchBoxRenderResults(results);
}

searchBox.addEventListener("input", searchBoxDebounce((e) => {
    searchBoxSearch(e.target.value);
}, 200));

searchBox.addEventListener("keydown", (e) => {
    if ((e.key === "Tab" || e.key === "ArrowDown") && searchBoxResults.children.length > 0) {
        e.preventDefault();
        searchBoxResults.children[0].focus();
    }
})

searchBoxResults.addEventListener("keydown", (e) => {
  const current = document.activeElement;
  if (current.parentElement !== searchBoxResults) return;

  if (e.key === "ArrowDown" && current.nextElementSibling) {
    e.preventDefault();
    current.nextElementSibling.focus();
  } else if (e.key === "ArrowUp") {
    e.preventDefault();
    if (current.previousElementSibling) {
      current.previousElementSibling.focus();
    } else {
      // go back to search box if at top
      searchBox.focus();
    }
  } else if (e.key === "Enter") {
    current.click(); // trigger selection
  }
});

document.addEventListener("click", (e) => {
    if (!searchBox.contains(e.target) && !searchBoxResults.contains(e.target)) {
        searchBoxResults.style.display = "none";
    }
});