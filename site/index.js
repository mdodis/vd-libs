const navbar = document.getElementById('mainnav');

window.addEventListener('scroll', () => {
    if (window.scrollY > 0) {
        navbar.classList.add('shadow'); // Add shadow when scrolled
    } else {
        navbar.classList.remove('shadow'); // Remove shadow at top
    }
});

document.querySelectorAll('pre.copy').forEach(pre => {
  // Create the copy button
  const button = document.createElement('button');
  button.textContent = 'Copy';
  button.className = 'btn btn-outline-secondary copy-btn';

  // Style the button (you can move this to CSS)
  button.style.position = 'absolute';
  button.style.top = '5px';
  button.style.right = '5px';
  button.style.padding = '5px 10px';
  button.style.fontSize = '12px';
  button.style.cursor = 'pointer';

  // Wrap <pre> in a container to allow absolute positioning
  const wrapper = document.createElement('div');
  wrapper.style.position = 'relative';
  pre.parentNode.insertBefore(wrapper, pre);
  wrapper.appendChild(pre);
  wrapper.appendChild(button);

  // Copy functionality
  button.addEventListener('click', () => {
    navigator.clipboard.writeText(pre.innerText).then(() => {
      button.textContent = 'Copied!';
      setTimeout(() => button.textContent = 'Copy', 2000);
    }).catch(err => console.error('Failed to copy: ', err));
  });
});
