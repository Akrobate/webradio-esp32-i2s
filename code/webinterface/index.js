let template_radio_station_list_item = null
let template_available_networks_list_item = null

const STATION_NAME_MAX_LENGTH = 30
const STATION_HOST_MAX_LENGTH = 1000


/**
 * INIT
 */
document.addEventListener('DOMContentLoaded', async () => {
    template_radio_station_list_item = document.getElementById('template-radio-station-list-item')
    template_available_networks_list_item = document.getElementById('template-available-networks-list-item')
    await loadAvailableNetworksList();
    await loadStationList();
});


async function loadStationList() {
    const dom_radio_stations_list = document.getElementById("radio-stations-list")
    dom_radio_stations_list.innerHTML = '';


    const stations_list = await serverGetStationList();
    stations_list.forEach((station, index) => {
        const new_radio_station_list_item = template_radio_station_list_item.content.cloneNode(true);

        new_radio_station_list_item.querySelector(".title").textContent = station.name;
        new_radio_station_list_item.querySelector(".subtitle").textContent = station.host;
        new_radio_station_list_item.querySelector(".list-item").dataset.index = index;

        dom_radio_stations_list.appendChild(new_radio_station_list_item);
    });
}



/**
 * STATIOn MANAGEMENT
 * @returns 
 */
async function addStation() {

    const station_name_input = document.getElementById("input-station-name");
    const station_host_input = document.getElementById("input-station-host");
        
    const station_name = station_name_input.value;
    const station_host = station_host_input.value;

    if (station_name.length == 0) {
        station_name_input.classList.add("error")
        return;
    } else {
        station_name_input.classList.remove("error")
    }

    if (!isValidUrl(station_host)) {
        station_host_input.classList.add("error")
        return;
    } else {
        station_host_input.classList.remove("error")
    }

    if (station_name.length > STATION_NAME_MAX_LENGTH) {
        station_name_input.classList.add("error")
        return;
    } else {
        station_name_input.classList.remove("error")
    }

    if (station_host.length > STATION_HOST_MAX_LENGTH) {
        station_host_input.classList.add("error")
        return;
    } else {
        station_host_input.classList.remove("error")
    }


    document.getElementById('station-manager').querySelector('.loader').classList.remove('hidden');

    station_host_input.value = "";
    station_name_input.value = "";

    const new_radio_station_list_item = template_radio_station_list_item.content.cloneNode(true);

    new_radio_station_list_item.querySelector(".title").textContent = station_name;
    new_radio_station_list_item.querySelector(".subtitle").textContent = station_host;

    await serverAddStation({name: station_name, host: station_host});
    await loadStationList();

    document.getElementById('station-manager').querySelector('.loader').classList.add('hidden');

}



async function deleteStation(elemnent) {
    console.log(elemnent);
    const index = elemnent.parentElement.parentElement.dataset.index;
    await serverRemoveStation(index);
    await loadStationList();
}


function editStation(elemnent) {
    console.log(elemnent);
    elemnent.parentElement.parentElement.remove();
}


function isValidUrl(string) {
    try {
      new URL(string);
      return true;
    } catch (_) {
      return false;
    }
}




async function loadAvailableNetworksList() {
    const dom_radio_stations_list = document.getElementById('available-networks-list')
    dom_radio_stations_list.innerHTML = '';


    const available_networks_list_list = await serverGetAvailableNetworksList();

    available_networks_list_list.forEach((station, index) => {
        const new_available_networks_list_list_item = template_available_networks_list_item.content.cloneNode(true);

        new_available_networks_list_list_item.querySelector(".title").textContent = station.ssid;
        new_available_networks_list_list_item.querySelector(".list-item").dataset.index = index;

        dom_radio_stations_list.appendChild(new_available_networks_list_list_item);
    });
}





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
    await wait(SERVER_LAG_SHORT);
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
    {
        ssid: "SSID 18",
        quality: 30,
        has_password: true,
    },
    {
        ssid: "SSID 19",
        quality: 40,
        has_password: false,
    },
    {
        ssid: "SSID 20",
        quality: 90,
        has_password: true,
    },
    {
        ssid: "SSID 21",
        quality: 100,
        has_password: false,
    },
    {
        ssid: "SSID 22",
        quality: 50,
        has_password: true,
    },
    {
        ssid: "SSID 23",
        quality: 20,
        has_password: false,
    },
    {
        ssid: "SSID 24",
        quality: 10,
        has_password: true,
    },
    {
        ssid: "SSID 25",
        quality: 80,
        has_password: true,
    }
]

async function serverGetAvailableNetworksList() {
    await wait(SERVER_LAG_SHORT)
    return _available_networks_list
}


