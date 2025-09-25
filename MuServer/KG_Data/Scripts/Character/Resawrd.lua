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
    
    -- Depuración: Mostrar información completa del jugador
    local playerName = GetPlayerName(playerIndex)
    local playerClass = GetPlayerClass(playerIndex)
    local playerLevel = GetPlayerLevel(playerIndex)
    print(string.format("[PlayTimeRewards] Jugador %s (ID: %d, Clase: %d, Nivel: %d) ha iniciado sesión", 
        playerName, playerIndex, playerClass, playerLevel))
end

-- Función para limpiar datos al desconectarse
function OnPlayerLogout(playerIndex)
    local playerName = GetPlayerName(playerIndex)
    print(string.format("[PlayTimeRewards] Jugador %s (ID: %d) se ha desconectado", playerName, playerIndex))
    playerPlayTime[playerIndex] = nil
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
                -- Verificar si el jugador tiene espacio en el inventario
                if GetInventoryEmptyCount(playerIndex) > 0 then
                    -- Entregar recompensa
                    GiveItem(playerIndex, reward.item.id, reward.item.count)
                    
                    -- Notificar al jugador
                    SendMessage(playerIndex, string.format(
                        "[Recompensa] ¡Has jugado %d minutos! Recibiste: %dx %s",
                        reward.minutes,
                        reward.item.count,
                        GetItemName(reward.item.id) or "Item"
                    ))
                    
                    print(string.format("[PlayTimeRewards] Recompensa entregada al jugador %s (ID: %d): %d minutos", 
                        playerName, playerIndex, currentTime))
                else
                    -- Notificar al jugador que no tiene espacio
                    SendMessage(playerIndex, "[Sistema] No tienes espacio en tu inventario para recibir la recompensa. Libera espacio y vuelve a conectar.")
                    
                    print(string.format("[PlayTimeRewards] Jugador %s (ID: %d) no tiene espacio para la recompensa", 
                        playerName, playerIndex))
                end
            end
        end
    end
end

-- Comando para verificar tiempo jugado
function OnPlayerCommand(playerIndex, command)
    if command == "/tiempo" then
        local currentTime = playerPlayTime[playerIndex] or 0
        local playerName = GetPlayerName(playerIndex)
        
        SendMessage(playerIndex, string.format("[Sistema] Has jugado %d minutos", currentTime))
        
        -- Mostrar próximas recompensas
        for _, reward in ipairs(rewards) do
            if currentTime < reward.minutes then
                local itemName = GetItemName(reward.item.id) or "Item"
                SendMessage(playerIndex, string.format(
                    "[Sistema] Próxima recompensa en %d minutos: %dx %s",
                    reward.minutes - currentTime,
                    reward.item.count,
                    itemName
                ))
                break
            end
        end
        
        return true
    end
    return false
end