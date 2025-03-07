'use strict'

const axios = require('axios');

// Configurations:
const API_URL = 'http://192.168.1.25';
const REQUEST_COUNT = 10000;


const url_list = [
    `${API_URL}`,
    `${API_URL}/index.js.gz`,
    `${API_URL}/repository.js.gz`,
    `${API_URL}/style.css.gz`,
];

async function sendRequest(url) {
    try {
        const startTime = Date.now();
        const response = await axios.get(url);
        const endTime = Date.now();
        console.log(`Request ${url} - Status: ${response.status}, Time: ${endTime - startTime} ms`);
    } catch (error) {
        console.log(`Request ${url} failed: ${error.message}`);
    }
}

async function runLoadTest() {
    for (let i = 0; i < REQUEST_COUNT; i++) {
        const promises = [];
        for (let url of url_list) {
            promises.push(sendRequest(url));
        }
        const startTime = Date.now();
        await Promise.all(promises);
        const endTime = Date.now();
        console.log(`==========> Request ${i} - Time: ${endTime - startTime} ms`);
    }
    console.log(`Finished sending ${REQUEST_COUNT} requests.`);
}

runLoadTest();

