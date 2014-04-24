update clan,player_buff set clanTitleAll = "0|1|" where player_buff.buffId = 0x68 and player_buff.data > 0 and player_buff.id = clan.leader;
