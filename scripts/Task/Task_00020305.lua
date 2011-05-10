--����Ľ�������
function Task_Accept_00020305()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 19 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(20305) or task:HasCompletedTask(20305) or task:HasSubmitedTask(20305) then
		return false;
	end
	if not task:HasSubmitedTask(20304) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00020305()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 19 then
		return false;
	end
	if task:HasAcceptedTask(20305) or task:HasCompletedTask(20305) or task:HasSubmitedTask(20305) then
		return false;
	end
	if not task:HasSubmitedTask(20304) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00020305()
	if GetPlayer():GetTaskMgr():HasCompletedTask(20305) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00020305(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(20305) == npcId and Task_Accept_00020305 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 20305
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "扫墓献花";
	elseif task:GetTaskSubmitNpc(20305) == npcId then
		if Task_Submit_00020305() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20305
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "扫墓献花";
		elseif task:HasAcceptedTask(20305) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20305
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "扫墓献花";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00020305_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "其实我真的很不想回忆起那场天魔之战的，记忆实在是太惨痛了，如果不是帝释天一直的蛮横无理，也不至于导致战争的爆发，使我们失去了儿子，也失去了悦意公主。";
	action.m_ActionMsg = "战争都是残酷的。";
	return action;
end

function Task_00020305_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "世界是不会一直这么平静下去的，帝释天从来没有为此悔改过，迟早有一天会有新的矛盾爆发，希望你能为此做好准备。现在你去把采来的那束山花插到文隣的墓上，以此寄托我们对他的思念。我看你刚刚去打采花怪兽似乎势力受了很大的损伤，任务完成以后你就去威海岛的龙宫，拜见文隣的师傅难陀罗大龙王吧，他会帮助你的。";
	action.m_ActionMsg = "原来大龙王在威海岛，我现在就过去。";
	return action;
end

function Task_00020305_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我也为失去文隣感到惋惜。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00020305_step_table = {
		[1] = Task_00020305_step_01,
		[2] = Task_00020305_step_02,
		[10] = Task_00020305_step_10,
		};

function Task_00020305_step(step)
	if Task_00020305_step_table[step] ~= nil then
		return Task_00020305_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00020305_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00020305() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15079, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(20305) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00020305_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15079,1) < 1 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(8911,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(20305) then
		return false;
	end

	if IsEquipTypeId(8911) then
		for k = 1, 1 do
			package:AddEquip(8911, 1);
		end
	else 
		package:AddItem(8911,1,1);
	end
	package:DelItemAll(15079,1);

	player:AddExp(1800);
	player:getCoin(2500);
	player:getTael(10);
	return true;
end

--��������
function Task_00020305_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15079,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(20305);
end
