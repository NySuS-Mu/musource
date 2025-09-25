-- Tabla para almacenar el tiempo de juego de cada jugador (en minutos)
local playerPlayTime = {}

-- Configuración de recompensas por tiempo
local rewards = {
    { minutes = 30, item = { id = 14, count = 1 } },   -- 30 min = 1 Jewel of Bless
    { minutes = 60, item = { id = 15, count = 1 } },   -- 60 min = 1 Jewel of Soul
    { minutes = 120, item = { id = 12, count = 1 } }, -- 120 min = 1 Box of Kundun
    { minutes = 180, item = { id = 13, count = 1 } }  -- 180 min = 1 Jewel of Creation
}

-- Mensaje de depuración al cargar el script
print("[PlayTimeRewards] Script cargado correctamente")

-- Función para inicializar el tiempo de juego al conectarse
function OnPlayerLogin(playerIndex)
    playerPlayTime[playerIndex] = 0
    SendMessage(playerIndex, "[Sistema de Recompensas] ¡Gana items por tiempo online! Revisa cada 30 minutos.")
    
    -- Depuración: Mostrar nombre del jugador
    local playerName = GetPlayerName(playerIndex)
    print("[PlayTimeRewards] Jugador " .. playerName .. " (ID: " .. playerIndex .. ") ha iniciado sesión")
end

-- Función para limpiar datos al desconectarse
function OnPlayerLogout(playerIndex)
    local playerName = GetPlayerName(playerIndex)
    playerPlayTime[playerIndex] = nil
    print("[PlayTimeRewards] Jugador " .. playerName .. " (ID: " .. playerIndex .. ") se ha desconectado")
end

-- Evento que se ejecuta cada minuto
function OnMinuteElapsed()
    print("[PlayTimeRewards] OnMinuteElapsed ejecutado")
    
    -- Recorrer todos los jugadores conectados
    for playerIndex, time in pairs(playerPlayTime) do
        -- Actualizar tiempo de juego
        playerPlayTime[playerIndex] = time + 1
        local currentTime = playerPlayTime[playerIndex]
        local playerName = GetPlayerName(playerIndex)
        
        print(string.format("[PlayTimeRewards] Jugador %s (ID: %d): %d minutos", playerName, playerIndex, currentTime))
        
        -- Verificar si el jugador alcanzó alguna recompensa
        for _, reward in ipairs(rewards) do
            if currentTime == reward.minutes then
                -- Entregar recompensa
                GiveItem(playerIndex, reward.item.id, reward.item.count)
                
                -- Notificar al jugador
                SendMessage(playerIndex, string.format(
                    "[Recompensa] ¡Has jugado %d minutos! Recibiste: %dx %s",
                    reward.minutes,
                    reward.item.count,
                    GetItemName(reward.item.id) or "Item"
                ))
                
                print(string.format("[PlayTimeRewards] Recompensa entregada al jugador %s (ID: %d): %d minutos", playerName, playerIndex, currentTime))
            end
        end
    end
end