
let keyboardInitialized = false;
let keyboardShown = false;

function activateKeyboard() {
    keyboardShown = true;
    const keyboardInput  = document.getElementById('keyboard');
    const keyboardOutput = document.getElementById('keyboard-output');
    const keyboardCode = keyboardOutput.querySelector('code');
    keyboardCode.textContent = '';
    
    if (keyboardInitialized) return;
    keyboardInitialized = true;
    
    function findVisualKeyForEvent(e){
        // match by code first, fall back to matching by printed key (case-insensitive)
        const byCode = document.querySelector(`[data-code="${e.code}"]`);
        if(byCode) return byCode;
        const candidate = Array.from(document.querySelectorAll('.key')).find(k=>{
            const dk = k.dataset.key;
            return dk && dk.toLowerCase() === e.key.toLowerCase();
        });
        return candidate || null;
    }
    
    function flashKey(keyEl) {
        keyEl.classList.add('pressed');
        setTimeout(() => keyEl.classList.remove('pressed'), 140);
    }
    
    function showKey(fwkey){
        const code = fwkey || 'Unknown';
        if (!(fwkey === "null" || fwkey === "Unknown")) {
            keyboardCode.textContent = code + "\n" + keyboardCode.textContent;
        }
        // keep only last 50 entries
        if(historyList.children.length > 50){ historyList.removeChild(historyList.lastChild); }
    }
    
    window.addEventListener('keydown', event => {
        if (!keyboardShown) return;
        event.preventDefault();
        const visual = findVisualKeyForEvent(event);
        if (visual) {
            flashKey(visual);
            showKey(visual.dataset.fwcode);
        }
        
    });
    
    keyboardInput.addEventListener('click', event => {
        const keyEl = event.target.closest('.key');
        flashKey(keyEl);
        showKey(keyEl.dataset.fwcode);
    });
}

document.getElementById('keyboard-backdrop').addEventListener('shown.bs.modal', function () {
    activateKeyboard();
});

document.getElementById('keyboard-backdrop').addEventListener('hidden.bs.modal', function () {
    keyboardShown = false;
});