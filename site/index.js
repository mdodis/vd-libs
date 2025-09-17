const navbar = document.getElementById('mainnav');

window.addEventListener('scroll', () => {
    if (window.scrollY > 0) {
        navbar.classList.add('shadow'); // Add shadow when scrolled
    } else {
        navbar.classList.remove('shadow'); // Remove shadow at top
    }
});