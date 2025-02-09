
local track = 0

BindUpdateFunction(function(data)

    if track >= 1 then
        print(track)
        local cellData = data.MapData[1]
        if cellData == 5 then
            data.MapData[1] = 2
        else
            data.MapData[1] = cellData + 1
        end
        track = 0
    end
    
    track = track + data.DeltaTime

    return data
end)
