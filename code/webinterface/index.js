let template_radio_station_list_item = null
let template_available_networks_list_item = null
let template_saved_networks_list_item = null


const STATION_NAME_MAX_LENGTH = 30
const STATION_HOST_MAX_LENGTH = 1000


/**
 * INIT
 */
document.addEventListener('DOMContentLoaded', async () => {
    template_radio_station_list_item = document.getElementById('template-radio-station-list-item')
    template_available_networks_list_item = document.getElementById('template-available-networks-list-item')
    template_saved_networks_list_item = document.getElementById('template-saved-networks-list-item')
    await loadStationList()
    await loadSavedNetworksList()
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
 * Station management
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

/**
 * Wifi management
 * @returns 
 */
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




async function loadSavedNetworksList() {
    const dom_saved_networks_list = document.getElementById('saved-networks-list')
    dom_saved_networks_list.innerHTML = '';

    const saved_networks_list = await serverGetSavedNetworksList();

    saved_networks_list.forEach((station, index) => {
        const new_saved_networks_list_list_item = template_saved_networks_list_item.content.cloneNode(true);

        new_saved_networks_list_list_item.querySelector(".title").textContent = station.ssid;
        new_saved_networks_list_list_item.querySelector(".list-item").dataset.index = index;

        dom_saved_networks_list.appendChild(new_saved_networks_list_list_item);
    });
}






