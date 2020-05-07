# infinitashook

A score event server for beatmania IIDX INFINITAS.

The creation of this project is detailed in the following three blog posts:

* https://aixxe.net/2019/06/iidx-score-data
* https://aixxe.net/2019/12/iidx-score-hook
* https://aixxe.net/2020/05/iidx-score-server

## Usage

### Server

- Compile using [CMake](https://cmake.org/).
- Load into `bm2dx.exe` using your preferred library loader.

### Client

- Run `npm install` or `yarn` from the client directory.
- Update `config.js` to contain your Discord Webhook URL.
- Start the client by running `node app.js` from the client directory.