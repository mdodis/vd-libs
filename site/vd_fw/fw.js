
let keyboardInitialized = false;
let keyboardShown = false;

const keyboardInput  = document.getElementById('keyboard-input60');
const keyboardOutput = document.getElementById('keyboard-output');
const keyboardCode = keyboardOutput.querySelector('code');
function activateKeyboard() {
    keyboardShown = true;
    keyboardCode.textContent = '';
    
    if (keyboardInitialized) return;
    keyboardInitialized = true;
    
    function findVisualKeyForEvent(e){
        // match by code first, fall back to matching by printed key (case-insensitive)
        const byCode = document.querySelector(`[data-code="${e.code}"]`);
        if(byCode) return byCode;
        const candidate = Array.from(keyboardInput.querySelectorAll('.key')).find(k=>{
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
    }
    
    window.addEventListener('keydown', event => {
        if (!keyboardShown) return;
        if (document.activeElement !== keyboardInput) return;
        // event.preventDefault();
        const visual = findVisualKeyForEvent(event);
        if (visual) {
            flashKey(visual);
            showKey(visual.dataset.fwcode);
        }
    });
    
    keyboardInput.addEventListener('click', event => {
        const keyEl = event.target.closest('.key');
        if (keyEl) {
            flashKey(keyEl);
            showKey(keyEl.dataset.fwcode);
        }
    });
}

document.getElementById('keyboard-backdrop').addEventListener('shown.bs.modal', function () {
    activateKeyboard();
    if (keyboardInput) {
        keyboardInput.setAttribute('tabindex', '0');
        keyboardInput.focus();
    }
});

document.getElementById('keyboard-backdrop').addEventListener('hidden.bs.modal', function () {
    keyboardShown = false;
});

function clearKeyboardOutput() {
    keyboardCode.textContent = '';
}