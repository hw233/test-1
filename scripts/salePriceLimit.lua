local serverTable = {
    [34] = 1,
    [72] = 1,
    [82] = 1,
    [87] = 1,
    [106] = 1,
    [114] = 1,
    [129] = 1,
    [138] = 1,
    [148] = 1,
    [158] = 1,
    [167] = 1,
    [173] = 1,
    [176] = 1,
    [179] = 1,
    [181] = 1,
    [184] = 1,
    [186] = 1,
    [188] = 1,
    [191] = 1,
    [194] = 1,
    [196] = 1,
    [198] = 1,
    [201] = 1,
    [204] = 1,
    [207] = 1,
    [210] = 1,
    [212] = 1,
    [214] = 1,
    [216] = 1,
    [219] = 1,
    [222] = 1,
    [224] = 1,
    [227] = 1,
    [230] = 1,
    [232] = 1,
    [234] = 1,
    [236] = 1,
    [238] = 1,
    [240] = 1,
    [242] = 1,
    [244] = 1,
    [249] = 1,
    [254] = 1,
    [257] = 1,
    [259] = 1,
    [261] = 1,
    [265] = 1,
    [269] = 1,
    [276] = 1,
    [281] = 1,
    [288] = 1,
    [291] = 1,
    [294] = 1,
    [297] = 1,
    [301] = 1,
    [305] = 1,
    [311] = 1,
    [318] = 1,
    [324] = 1,
    [327] = 1,
    [333] = 1,
    [341] = 1,
    [346] = 1,
    [352] = 1,
    [358] = 1,
    [362] = 1,
    [366] = 1,
    [370] = 1,
    [373] = 1,
    [376] = 1,
    [379] = 1,
    }

function isSalePriceLimitServer(serverNum)

    print("xxxx\n")
	local limit = serverTable[serverNum];
	if limit == nil then
		return false;
    end

    return limit;
end
