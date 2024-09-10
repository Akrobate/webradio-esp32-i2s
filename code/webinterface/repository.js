


async function serverGetStationList() {
    try {
        const result = await fetch('/api/streams',
            {
                method: 'GET',
            }
        )
        return result.json()
    } catch (error) {
        console.log('serverGetStationList - Error:', error)
        return []
    }
}


async function serverAddStation(input) {
    const {
        host,
        name,
    } = input
    try {
        let form_data = new FormData()
        form_data.append('host', host)
        form_data.append('name', name)
        await fetch('/api/streams',
            {
                body: form_data,
                method: 'POST',
            }
        )
    } catch (error) {
        console.log('serverAddStation - Error:', error)
    }
}


async function serverRemoveStation(index) {
    try {
        await fetch(`/api/streams/${index}`,
            {
                method: 'DELETE',
            }
        )
    } catch (error) {
        console.log('serverRemoveStation - Error:', error)
    }
}

async function serverEditStation(index, input) {
    const {
        host,
        name,
    } = input
    try {
        let form_data = new FormData()
        form_data.append('host', host)
        form_data.append('name', name)
        await fetch(`/api/streams/${index}`,
            {
                body: form_data,
                method: 'PATCH',
            }
        )
    } catch (error) {
        console.log('serverEditStation - Error:', error)
    }
}


async function serverMoveUpStation(index) {
    try {
        let form_data = new FormData()
        form_data.append('index', index)
        await fetch('/api/streams/position-up',
            {
                body: form_data,
                method: 'POST',
            }
        )
    } catch (error) {
        console.log('serverMoveUpStation - Error:', error)
    }
}


async function serverMoveDownStation(index) {
    try {
        let form_data = new FormData()
        form_data.append('index', index)
        await fetch('/api/streams/position-down',
            {
                body: form_data,
                method: 'POST',
            }
        )
    } catch (error) {
        console.log('serverMoveDownStation - Error:', error)
    }
}


async function serverGetAvailableNetworksList() {
    try {
        const result = await fetch('/api/available-networks',
            {
                method: 'GET',
            }
        )
        return result.json()
    } catch (error) {
        console.log('serverGetAvailableNetworksList - Error:', error)
        return []
    }
}


async function serverGetSavedNetworksList() {
    try {
        const result = await fetch('/api/credentials',
            {
                method: 'GET',
            }
        )
        return result.json()
    } catch (error) {
        console.log('serverGetSavedNetworksList - Error:', error)
        return []
    }
}


async function serviceAddSavedNetwork(network) {
    const {
        ssid,
        password,
    } = network
    try {
        let form_data = new FormData()
        form_data.append('ssid', ssid)
        form_data.append('password', password)
        await fetch('/api/credentials',
            {
                body: form_data,
                method: 'POST',
            }
        )
    } catch (error) {
        console.log('serviceAddSavedNetwork - Error:', error)
    }
}


async function serverRemoveSavedNetwork(ssid) {
    try {
        let form_data = new FormData()
        form_data.append('ssid', ssid)
        await fetch('/api/credentials',
            {
                body: form_data,
                method: 'DELETE',
            }
        )
    } catch (error) {
        console.log('serverRemoveSavedNetwork - Error:', error)
    }
}


async function serverGetInfo() {
    try {
        const result = await fetch('/api/info',
            {
                method: 'GET',
            }
        )
        return result.json()
    } catch (error) {
        console.log('serverGetInfo - Error:', error)
        return []
    }
}



async function serverSaveConfigurations(network) {
    const {
        ntp_server_host,
        gmt_offset_sec,
        daylight_offset_sec,
    } = network
    try {
        let form_data = new FormData()

        if (ntp_server_host !== undefined) {
            form_data.append('ntp_server_host', ntp_server_host)
        }
        if (gmt_offset_sec !== undefined) {
            form_data.append('gmt_offset_sec', gmt_offset_sec)
        }
        if (daylight_offset_sec !== undefined) {
            form_data.append('daylight_offset_sec', daylight_offset_sec)
        }

        await fetch('/api/configurations',
            {
                body: form_data,
                method: 'POST',
            }
        )
    } catch (error) {
        console.log('serverSaveConfigurations - Error:', error)
    }
}
