class VWindowComponent extends HTMLElement {
    constructor() {
        super();
        this.mouse = {x: 0, y: 0};
        this.metadata = {x: 0, y: 0, w: 860, h: 400};
    }

    static get observedAttributes() {
        return ['title'];
    }

    set title(value) {
        this._title = value;
        if (this.header) {
            this.updateHeader();
            this.setAttribute('title', value);
        }
    }

    static removeAllWindowFocus() {
        document.querySelectorAll('v-window').forEach((windowEl) => {
            windowEl.removeAttribute('focused');
        });
    }

    connectedCallback() {
        if (!this.header) {
            this.setupHeader();
            this.setupContent();
            this.innerHTML = null;
            this.appendChild(this.header);
            this.appendChild(this.content);
            this.setupResizer();
        }
        this.updateMetaData();
    }

    updateHeader() {
        this.header.querySelector('v-window-headline').innerHTML = this._title ?? "Unknown Window";
    }

    attributeChangedCallback(name, oldValue, newValue) {
        if (name === 'title' && this._title !== newValue) {
            this.title = newValue;
        }
    }

    setupContent() {
        this.content = document.createElement("v-window-content");
        const children = this.children;
        while (children.length) {
            this.content.appendChild(children[0]);
        }
    }

    setupHeader() {
        this.header = document.createElement('v-window-header');
        const headline = document.createElement('v-window-headline');
        this.header.appendChild(headline);
        this.updateHeader();
        const buttons = document.createElement('div');
        buttons.classList.add('window-buttons');
        this.getItems().forEach((item) => {
            const button = document.createElement('span');
            button.classList.add('button', item.class ?? 'default');
            button.innerHTML = item.text;
            buttons.appendChild(button);
            button.addEventListener('click', () => this.clickWindowButton(item));
        })
        this.setupEvents();
        this.header.appendChild(buttons);
    }

    setupResizer() {
        const resizer = this.resizer = document.createElement('v-window-resizer');
        let data = {x: 0, y: 0};
        const resizingFunction = (ev) => {
            this.metadata.w += ev.clientX - data.x;
            this.metadata.h += ev.clientY - data.y;
            this.metadata.w = Math.max(this.metadata.w, 200);
            this.metadata.h = Math.max(this.metadata.h, 100);
            this.updateMetaData();
            ev.preventDefault();
            ev.stopPropagation();
            data = {x: ev.clientX, y: ev.clientY};
        };
        this.addMultiListener(resizer, 'mousedown touchstart', (ev) => {
            ev.preventDefault();
            ev.stopPropagation();
            this.setAttribute('resizing', "1");
            data = {x: ev.clientX, y: ev.clientY};
            this.addMultiListener(window, 'mousemove touchmove', resizingFunction);
            this.addMultiListener(window, 'mouseup touchend', () => {
                this.removeAttribute('resizing');
                this.removeMultiListener(window, 'mousemove touchmove', resizingFunction);
            }, {once: true});
        });
        this.appendChild(resizer);
    }

    addMultiListener(element, selector, cb, options = {}) {
        selector.split(" ").forEach(eventBind => element.addEventListener(eventBind, cb, options));
    }

    removeMultiListener(element, selector, cb, options = {}) {
        selector.split(" ").forEach(eventBind => element.removeEventListener(eventBind, cb, options));
    }

    clickWindowButton(item) {
        return item.cb && item.cb();
    }

    updateMetaData() {
        this.metadata.x = Math.max(this.metadata.x, 0);
        this.metadata.y = Math.max(this.metadata.y, 0);
        this.style.transform = `translate(${this.metadata.x}px,${this.metadata.y}px)`;
        this.style.maxWidth = this.metadata.w + 'px';
        this.style.maxHeight = this.metadata.h + 'px';
        this.removeAttribute('maximized');
    }

    setupEvents() {
        this.addMultiListener(this, 'mousedown touchstart', () => {
            VWindowComponent.removeAllWindowFocus();
            this.setAttribute('focused', '1');
        }, {passive: true});

        this.addMultiListener(this.header, 'mousedown touchstart', (ev) => {
            if (ev.target.closest('.window-buttons')) {
                return;
            }
            ev.preventDefault();
            ev.stopPropagation();
            this.isDragging = true;
            this.mouse = {x: ev.clientX, y: ev.clientY};

            this.setAttribute('resizing', "1");
            this.addMultiListener(window, 'mouseup touchend', (ev) => {
                this.isDragging = false;
                this.removeAttribute('resizing');
            }, {once: true});
        });

        this.addMultiListener(window, 'mousemove touchmove', (ev) => {
            if (this.isDragging) {
                this.metadata.x += ev.clientX - this.mouse.x;
                this.metadata.y += ev.clientY - this.mouse.y;
                this.mouse = {x: ev.clientX, y: ev.clientY};
                this.updateMetaData();
            }
        }, {passive: true});
    }

    getItems() {
        return [{
            class: 'fullscreen', text: '&#x26F6;', cb: () => {
                if (this.hasAttribute('maximized')) {
                    this.updateMetaData();
                } else {
                    this.setAttribute('maximized', 'true');
                    this.style.transform = `translate(0px,0px)`;
                    this.style.maxWidth = '100%';
                    this.style.maxHeight = '100%';
                }
            }
        }, {
            class: 'close', text: "&#x2715;", cb: () => {
                this.parentElement.removeChild(this);
            }
        }]
    }
}

customElements.define("v-window", VWindowComponent);