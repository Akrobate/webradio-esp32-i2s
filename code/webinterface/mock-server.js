/******************************************************************************************
  
 *                                       MOCK SERVER
  
******************************************************************************************/
const _stations_list = [
    {
        name: "Radio 1",
        host: "orem ipsum sic test Lorem iporem ipsum sic test Lorem iporem ipsum sic test Lorem iporem ip"
    },
    {
        name: "Radio 2",
        host: "https://css-tricks.com/snippets/css/a-guide-to-flexbox/"
    },
    {
        name: "Radio 3",
        host: "radio3.com"
    },
]

const SERVER_LAG_LONG = 1000;
const SERVER_LAG_SHORT = 500;

async function wait(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}

async function serverGetStationList() {
    await wait(SERVER_LAG_LONG);
    return _stations_list;
}

async function serverRemoveStation(station_index) {
    await wait(SERVER_LAG_LONG);
    _stations_list.splice(station_index, 1);
    return ;
}

async function serverEditStation(station_index, station) {
    _stations_list[station_index] = station;
    return Promise.resolve();
}

async function serverAddStation(station) {
    await wait(SERVER_LAG_SHORT + 3000);
    _stations_list.push(station);
    return Promise.resolve();
}


const _available_networks_list = [
    {
        ssid: "SSID 1",
        quality: 100,
        has_password: false,
    },
    {
        ssid: "SSID 2",
        quality: 50,
        has_password: true,
    },
    {
        ssid: "SSID 3",
        quality: 20,
        has_password: false,
    },
    {
        ssid: "SSID 4",
        quality: 10,
        has_password: true,
    },
    {
        ssid: "SSID 5",
        quality: 80,
        has_password: false,
    },
    {
        ssid: "SSID 6",
        quality: 70,
        has_password: true,
    },
    {
        ssid: "SSID 7",
        quality: 60,
        has_password: false,
    },
    {
        ssid: "SSID 8",
        quality: 30,
        has_password: true,
    },
    {
        ssid: "SSID 9",
        quality: 40,
        has_password: false,
    },
    {
        ssid: "SSID 10",
        quality: 90,
        has_password: true,
    },
    {
        ssid: "SSID 11",
        quality: 100,
        has_password: false,
    },
    {
        ssid: "SSID 12",
        quality: 50,
        has_password: true,
    },
    {
        ssid: "SSID 13",
        quality: 20,
        has_password: false,
    },
    {
        ssid: "SSID 14",
        quality: 10,
        has_password: true,
    },
    {
        ssid: "SSID 15",
        quality: 80,
        has_password: false,
    },
    {
        ssid: "SSID 16",
        quality: 70,
        has_password: true,
    },
    {
        ssid: "SSID 17",
        quality: 60,
        has_password: false,
    },
]


const _saved_networks_list = [
    {
        ssid: "SSID 4",
        quality: 10,
    },
    {
        ssid: "SSID 5",
        quality: 80,
    },
    {
        ssid: "SSID 6",
        quality: 70,
    }
]




async function serverGetAvailableNetworksList() {
    await wait(SERVER_LAG_SHORT)
    return _available_networks_list
}


async function serverGetSavedNetworksList() {
    await wait(SERVER_LAG_SHORT)
    return _saved_networks_list
}


async function serverRemoveSavedNetwork(network_index) {
    await wait(SERVER_LAG_SHORT)
    _saved_networks_list.splice(network_index, 1)
    return Promise.resolve()
}

async function serviceAddSavedNetwork(network) {
    await wait(SERVER_LAG_SHORT)
    _saved_networks_list.push(network)
    return Promise.resolve()
}