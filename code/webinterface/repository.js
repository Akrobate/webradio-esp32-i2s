


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



async function serverSaveConfiguration(network) {
    const {
        ntp_server_host,
        gmt_offset,
        daylight_offset,
    } = network
    try {
        let form_data = new FormData()

        form_data.append('ntp_server_host', ntp_server_host)
        form_data.append('gmt_offset', gmt_offset)
        form_data.append('daylight_offset', daylight_offset)

        await fetch('/api/configurations',
            {
                body: form_data,
                method: 'POST',
            }
        )
    } catch (error) {
        console.log('serverSaveConfiguration - Error:', error)
    }
}
