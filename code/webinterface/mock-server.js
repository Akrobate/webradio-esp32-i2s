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
    console.log('serverGetStationList')
    await wait(SERVER_LAG_LONG);
    return _stations_list;
}

async function serverRemoveStation(station_index) {
    console.log('serverRemoveStation', station_index)
    await wait(SERVER_LAG_LONG);
    _stations_list.splice(station_index, 1);
    return ;
}

async function serverEditStation(station_index, station) {
    console.log('serverEditStation', station_index, station)
    _stations_list[station_index] = station;
    return Promise.resolve();
}

async function serverAddStation(station) {
    console.log('serverAddStation', station)
    await wait(SERVER_LAG_SHORT + 3000);
    _stations_list.push(station);
    return Promise.resolve();
}

async function serverStationMoveUp(index) {
    console.log('serverStationMoveUp', index)
    await wait(SERVER_LAG_SHORT);
    const tmp = _stations_list[index]
    _stations_list[index] = _stations_list[index - 1]
    _stations_list[index - 1] = tmp
    return Promise.resolve();
}


async function serverStationMoveDown(index) {
    console.log('serverStationMoveDown', index)
    await wait(SERVER_LAG_SHORT);
    const tmp = _stations_list[index]
    _stations_list[index] = _stations_list[index + 1]
    _stations_list[index + 1] = tmp
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
    }
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

const _saved_configurations = {
    gmt_offset_sec: 3600,
    daylight_offset_sec: 3600,
    ntp_server_host: 'http://pool.ntp.org'
}

async function serverGetAvailableNetworksList() {
    console.log('serverGetAvailable')
    await wait(SERVER_LAG_SHORT)
    return _available_networks_list
}


async function serverGetSavedNetworksList() {
    console.log('serverGetSavedNetworksList')
    await wait(SERVER_LAG_SHORT)
    return _saved_networks_list
}


async function serverRemoveSavedNetwork(ssid) {
    console.log('serverRemoveSavedNetwork', ssid)
    await wait(SERVER_LAG_SHORT)
    const network_index = _saved_networks_list.findIndex(network => network.ssid === ssid)
    _saved_networks_list.splice(network_index, 1)
    return Promise.resolve()
}

async function serviceAddSavedNetwork(network) {
    await wait(SERVER_LAG_SHORT)
    _saved_networks_list.push(network)
    return Promise.resolve()
}

async function serverGetInfo() {
    await wait(SERVER_LAG_LONG)
    return Promise.resolve({
        access_point_ssid: 'ESP32-Access-Point',
        temperature: 21.2000009,
        pressure: 1020.2000009,
        total_free_bytes: 123000,
        minimum_free_bytes: 122000,
        is_connected_to_wifi: false,
        is_connecting_to_wifi: true,
        connected_to_ssid: 'SSID X',
        local_ip: '192.168.1.176',
        date_time: '2023-07-16T12:30:00',
        date_time_configured: false,
        ..._saved_configurations,
    })
}

async function serverSaveConfigurations(input) {

    console.log('serverSaveConfigurations', input)
    await wait(SERVER_LAG_LONG)


    const {
        gmt_offset_sec,
        daylight_offset_sec,
        ntp_server_host,
    } = input

    if (gmt_offset_sec) {
        _saved_configurations.gmt_offset_sec = gmt_offset_sec
    }
    if (daylight_offset_sec) {
        _saved_configurations.daylight_offset_sec = daylight_offset_sec
    }
    if (ntp_server_host) {
        _saved_configurations.ntp_server_host = ntp_server_host
    }


    await wait(SERVER_LAG_LONG)
    return Promise.resolve(_saved_configurations)
}