--力量 体魄、智力、敏捷、命中、闪躲、暴击、破击、反击、行动、生命、攻击、防御
local factor1  = {2.5, 6.5, 1.5, 2.5, 0.10, 0.10, 0.15, 0.1, 0.20,  0.001,  75, 15, 30}		--1
local factor2  = {6.5, 2.5, 1.5, 2.5, 0.10, 0.075, 0.20, 0.20, 0.1,  0.002,  30, 25, 20}	--3
local factor3  = {6.5, 2.5, 1.5, 2.5, 0.15, 0.075, 0.20, 0.20, 0.15, 0.0015, 40, 20, 25}	--5
local factor4 = {1.5, 4.0, 1.0, 1.5, 0.10, 0.10, 0.15, 0.1, 0.20,  0.001,  50, 10, 25};	--猛将
local factor5 = {4.0, 1.5, 1.0, 1.5, 0.10, 0.075, 0.20, 0.20, 0.1,  0.002,  20, 20, 15}; --谋士
local factor6 = {4.0, 1.5, 1.0, 1.5, 0.15, 0.075, 0.20, 0.20, 0.15, 0.0015, 25, 15, 20}; --刺客
local factor7 = {1.25, 2.0, 0.5, 0.75, 0.10, 0.075, 0.10, 0.05, 0.15, 0.0010, 30, 10, 15}; --猛将
local factor8 = {2.0, 0.75, 0.5, 1.25, 0.10, 0.050, 0.15, 0.15, 0.05, 0.0015, 10, 15, 10}; --谋士
local factor9 = {2.0, 0.75, 0.5, 1.25, 0.15, 0.050, 0.15, 0.15, 0.10, 0.0010, 15, 10, 10}; --刺客

local FactorTable = { {[1] = factor1, [3] = factor2, [5] = factor3}, {[1] = factor4, [3] = factor5, [5] = factor6}, {[1] = factor7, [3] = factor8, [5] = factor9} };

function getFactor(klass, career, level)
	local newFactor = {};
	table.foreach(FactorTable[klass][career], function(i, v) table.insert(newFactor, v * level) end);
	return newFactor;
end


local clanTotemAssistBoss = { 4930, 4931, 4910, 4912, 4923, 4914, 4917, 4920, 4922, 4928, 4916, 4927 };
local clanSouthBoss = { 4910, 4911, 4912, 4913, 4914, 4915, 4916, 4917, 4918, 4919, 4930, 4931 };
local clanNorthBoss = { 4920, 4921, 4922, 4923, 4924, 4925, 4926, 4927, 4928, 4929, 4930, 4931 };
local clanSouthGuarderBoss = 4934;
local clanNorthGuarderBoss = 4932;
local clanTotemGuarderBoss = 4933;

function getClanTotemAssistBoss()
        return clanTotemAssistBoss;
end

function getClanSouthBoss()
        return clanSouthBoss;
end

function getClanNorthBoss()
        return clanNorthBoss;
end

function getClanSouthGuarderBoss()
        return clanSouthGuarderBoss;
end

function getClanNorthGuarderBoss()
        return clanNorthGuarderBoss;
end

function getClalTotemGuarderBoss()
        return clanTotemGuarderBoss;
end