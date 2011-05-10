--����Ľ�������
function Task_Accept_00030302()
	local player = GetPlayer();
	if player:GetLev() < 34 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30302) or task:HasCompletedTask(30302) or task:HasSubmitedTask(30302) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30301) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30301) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030302()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 34 then
		return false;
	end
	if task:HasAcceptedTask(30302) or task:HasCompletedTask(30302) or task:HasSubmitedTask(30302) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30301) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30301) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030302()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30302) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030302(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30302) == npcId and Task_Accept_00030302 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30302
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "打击鬼族";
	elseif task:GetTaskSubmitNpc(30302) == npcId then
		if Task_Submit_00030302() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30302
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "打击鬼族";
		elseif task:HasAcceptedTask(30302) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30302
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "打击鬼族";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030302_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "这些无聊的鬼族啊，就算再装下去，也会被人看穿他们那邪恶的面目，你这新来的天族勇士，我看你能力和天赋都还不错，只是山海大陆的险恶你还没有完全领教，还是很需要锻炼的。";
	action.m_ActionMsg = "请前辈指导。";
	return action;
end

function Task_00030302_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你就先去教训下那些在战场废墟的怨灵鬼魂吧，记得合理安排你的阵型和将领，才能占据战斗的优势。";
	action.m_ActionMsg = "多谢前辈指教，我这就去，以后还请多多赐教。";
	return action;
end

function Task_00030302_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "年轻人，动作还挺快的。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030302_step_table = {
		[1] = Task_00030302_step_01,
		[2] = Task_00030302_step_02,
		[10] = Task_00030302_step_10,
		};

function Task_00030302_step(step)
	if Task_00030302_step_table[step] ~= nil then
		return Task_00030302_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030302_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030302() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30302) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00030302_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8917,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(30302) then
		return false;
	end

	if IsEquipTypeId(8917) then
		for k = 1, 1 do
			package:AddEquip(8917, 1);
		end
	else 
		package:AddItem(8917,1,1);
	end

	player:AddExp(5000);
	player:getCoin(6800);
	player:getTael(20);
	return true;
end

--��������
function Task_00030302_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30302);
end
