# API REST - Bloom Filter Backend

## Endpoints

### POST /check
- **Body:** `{ "url": "..." }`
- **Respuesta:**
  - `{ "malicious": true }` (la URL podría estar en la blacklist, posible falso positivo)
  - `{ "malicious": false, "reason": "URL not found in blacklist" }` (la URL no está en el filtro)
- **Notas:**
  - El resultado puede ser un falso positivo, nunca un falso negativo.
  - Siempre responde con CORS y Content-Type: application/json.

### POST /api/verify
- **Body:** `{ "url": "..." }`
- **Respuesta:**
  - `{ "malicious": true, "method": "verify" }` (la URL está exactamente en la blacklist)
  - `{ "malicious": false, "reason": "URL not found in real blacklist", "method": "verify" }`
- **Notas:**
  - Esta comprobación es exacta, sin falsos positivos.
  - Útil para mostrar al usuario la diferencia entre la comprobación probabilística y la exacta.

### CORS y Preflight
- Todos los endpoints responden con las cabeceras CORS necesarias.
- Se recomienda usar POST y Content-Type: application/json.

## Ejemplo de uso

```bash
curl -X POST http://localhost:18080/check -H "Content-Type: application/json" -d '{"url":"http://ejemplo.com"}'
curl -X POST http://localhost:18080/api/verify -H "Content-Type: application/json" -d '{"url":"http://ejemplo.com"}'
```

## Notas sobre el Bloom Filter
- El campo `malicious` en `/check` significa "posiblemente maliciosa" (puede ser falso positivo).
- El campo `malicious` en `/api/verify` es exacto.
