# API Documentation

## Endpoints

### 1. `GET /api/volume`
**Description:**
Returns the current volume level.

**Response:**
- `200 OK`: JSON object containing the current volume.

**Example Response:**
```json
{
  "volume": 50
}
```

---

### 2. `POST /api/volume`
**Description:**
Sets the volume to a specified level.

**Request Parameters:**
- `volume` (integer, required): The new volume level (0-100).

**Response:**
- `201 Created`: Volume successfully updated.
- `400 Bad Request`: Invalid volume value.

**Example Request:**
```json
{
  "volume": 75
}
```

**Example Response:**
```json
{
  "volume": 75
}
```

---

### 3. `GET /api/available-networks`
**Description:**
Returns a list of available Wi-Fi networks.

**Response:**
- `200 OK`: JSON array of available networks.

**Example Response:**
```json
[
  { "ssid": "Network1", "signal": -45 },
  { "ssid": "Network2", "signal": -60 }
]
```

---

### 4. `PATCH /api/streams/{index}`
**Description:**
Updates a specific stream.

**Path Parameters:**
- `index` (integer, required): The index of the stream to update.

**Request Parameters:**
- `name` (string, optional): The new name of the stream.
- `url` (string, optional): The new URL of the stream.

**Response:**
- `200 OK`: Stream successfully updated.
- `400 Bad Request`: Invalid request or index out of range.

**Example Request:**
```json
{
  "name": "New Stream Name",
  "url": "http://example.com/stream"
}
```

**Example Response:**
```json
{
  "index": 1,
  "name": "New Stream Name",
  "url": "http://example.com/stream"
}
```

---

### 5. `DELETE /api/streams/{index}`
**Description:**
Deletes a specific stream.

**Path Parameters:**
- `index` (integer, required): The index of the stream to delete.

**Response:**
- `200 OK`: Stream successfully deleted.
- `400 Bad Request`: Invalid index.

**Example Response:**
```json
{
  "message": "Stream deleted successfully"
}
```

---

### 6. `GET /api/streams`
**Description:**
Returns a list of all streams.

**Response:**
- `200 OK`: JSON array of streams.

**Example Response:**
```json
[
  { "index": 0, "name": "Stream1", "url": "http://example.com/1" },
  { "index": 1, "name": "Stream2", "url": "http://example.com/2" }
]
```

