let radio_station_list_item_template = null;


const STATION_NAME_MAX_LENGTH = 30;
const STATION_HOST_MAX_LENGTH = 1000;

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

const SERVER_LAG = 1000;

async function wait(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}


async function serverGetStationList() {
    await wait(SERVER_LAG);
    return _stations_list;
}


async function serverRemoveStation(station_index) {
    await wait(SERVER_LAG);
    _stations_list.splice(station_index, 1);
    return ;
}

async function serverEditStation(station_index, station) {
    _stations_list[station_index] = station;
    return Promise.resolve();
}

async function serverAddStation(station) {
    await wait(SERVER_LAG);
    _stations_list.push(station);
    return Promise.resolve();
}



document.addEventListener('DOMContentLoaded', async () => {
    radio_station_list_item_template = document.getElementById("template-radio-station-list-item");
    console.log("here");

    await loadStationList();
});


async function loadStationList() {
    const dom_radio_stations_list = document.getElementById("radio-stations-list")
    dom_radio_stations_list.innerHTML = '';


    const stations_list = await serverGetStationList();
    stations_list.forEach((station, index) => {
        const new_radio_station_list_item = radio_station_list_item_template.content.cloneNode(true);

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
        console.log('fdfsdffsd')
        return;
    } else {
        station_host_input.classList.remove("error")
    }


    document.getElementById('station-manager').querySelector('.loader').classList.remove('hidden');

    station_host_input.value = "";
    station_name_input.value = "";

    const new_radio_station_list_item = radio_station_list_item_template.content.cloneNode(true);

    new_radio_station_list_item.querySelector(".title").textContent = station_name;
    new_radio_station_list_item.querySelector(".subtitle").textContent = station_host;

    await serverAddStation({name: station_name, host: station_host});
    await loadStationList();

    document.getElementById('station-manager').querySelector('.loader').classList.add('hidden');

    // document.getElementById("radio-stations-list").appendChild(new_radio_station_list_item);
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
    } catch (err) {
      return false;
    }
}


