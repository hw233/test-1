--����Ľ�������
function Task_Accept_00010104()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 5 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(10104) or task:HasCompletedTask(10104) or task:HasSubmitedTask(10104) then
		return false;
	end
	if not task:HasSubmitedTask(10103) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00010104()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 5 then
		return false;
	end
	if task:HasAcceptedTask(10104) or task:HasCompletedTask(10104) or task:HasSubmitedTask(10104) then
		return false;
	end
	if not task:HasSubmitedTask(10103) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00010104()
	if GetPlayer():GetTaskMgr():HasCompletedTask(10104) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00010104(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(10104) == npcId and Task_Accept_00010104 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 10104
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "复仇之路";
	elseif task:GetTaskSubmitNpc(10104) == npcId then
		if Task_Submit_00010104() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10104
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "复仇之路";
		elseif task:HasAcceptedTask(10104) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10104
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "复仇之路";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00010104_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士，我们相信你即将成为一名非常了不起的天族勇士，要想继续成长，你首先要了解我们的敌人魔族，他们盘踞在地界，是一个嗜血狂暴的种族。";
	action.m_ActionMsg = "我在听着，请继续。";
	return action;
end

function Task_00010104_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "魔族的族众嗜血好战，就是他们曾经挑起了毁灭性的天魔之战，你要记住这场战争带来的仇恨。在胜神洲的监狱里，关押着我们捉拿到的魔族的俘虏，杀死他们吧，他们不值得继续活着。然后你就有资格去拜见我们的战神忉利天了，他在位于东边的胜神洲的国清殿里。";
	action.m_ActionMsg = "把他们交给我吧，我一定不会让他们好受。";
	return action;
end

function Task_00010104_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "梵天交给你的任务你完成的非常好，真是天族有前途的勇士。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00010104_step_table = {
		[1] = Task_00010104_step_01,
		[2] = Task_00010104_step_02,
		[10] = Task_00010104_step_10,
		};

function Task_00010104_step(step)
	if Task_00010104_step_table[step] ~= nil then
		return Task_00010104_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00010104_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00010104() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(10104) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00010104_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(4,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(10104) then
		return false;
	end

	if IsEquipTypeId(4) then
		for k = 1, 1 do
			package:AddEquip(4, 1);
		end
	else 
		package:AddItem(4,1,1);
	end

	player:AddExp(600);
	player:getCoin(650);
	return true;
end

--��������
function Task_00010104_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(10104);
end
