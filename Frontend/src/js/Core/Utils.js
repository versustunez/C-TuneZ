const $ = (selector, element = document) => element.querySelector(selector);
const $$ = (selector, element = document) => element.querySelectorAll(selector);
const sleep = (ms) => new Promise(resolve => window.setTimeout(resolve, ms));