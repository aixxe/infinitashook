'use strict';

const path = require('path');
const fs = require('fs').promises;
const zmq = require('zeromq/v5-compat');
const jconv = require('jconv');
const puppeteer = require('puppeteer');
const discordjs = require('discord.js');

const config = require('./config');

const client = zmq.socket('sub');
const hook = new discordjs.WebhookClient(config.discord.webhookId, config.discord.webhookToken);

let browser, page, score = null;

async function screenshot(data) {
    await page.reload({ waitUntil: 'domcontentloaded' });
    await page.evaluate(data => {
        window.score = data;
        new Vue({el: '#scorecard'});
    }, data);

    // capture a screenshot and return as a buffer
    return await page.screenshot({ omitBackground: true });
}

client.on('message', async (data) => {
    let message = JSON.parse(data);

    if (message.event === 'score') {
        // Music title and artist are arrays of bytes. Convert encoding from SHIFT-JIS to UTF-8 here.
        message.data.music.title = jconv.decode(Buffer.from(message.data.music.title), 'SJIS');
        message.data.music.artist = jconv.decode(Buffer.from(message.data.music.artist), 'SJIS');

        score = message.data;
    }

    if (message.event === 'input') {
        // only post if the '0' key was pressed
        if (message.data.keys.indexOf(config.discord.submitKey) === -1)
            return false;

        // make sure there's a valid score ready to be posted
        if (score === null)
            return false;

        // copy the score, then clear it so it can't be posted twice
        const data = {...score}; score = null;

        // generate an image from the score
        const image = await screenshot(data);

        // save image locally & post to webhook
        await fs.writeFile(`./cards/${ Date.now() }.png`, image);
        await hook.send(new discordjs.MessageAttachment(image, 'score.png'));
    }
});

async function init() {
    browser = await puppeteer.launch();
    page = await browser.newPage();

    await page.goto('file:' + path.resolve('external', 'iidx-scorecard', 'template.html'));
    await page.setViewport({ width: 400, height: 600 });

    client.subscribe('');
    client.connect(config.client.address);
}

init();