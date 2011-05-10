--����Ľ�������
function Task_Accept_00040302()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 10 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40302) or task:HasCompletedTask(40302) or task:HasSubmitedTask(40302) then
		return false;
	end
	if not task:HasSubmitedTask(40301) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00040302()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 10 then
		return false;
	end
	if task:HasAcceptedTask(40302) or task:HasCompletedTask(40302) or task:HasSubmitedTask(40302) then
		return false;
	end
	if not task:HasSubmitedTask(40301) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00040302()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40302) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00040302(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40302) == npcId and Task_Accept_00040302 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40302
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "拜访";
	elseif task:GetTaskSubmitNpc(40302) == npcId then
		if Task_Submit_00040302() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40302
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "拜访";
		elseif task:HasAcceptedTask(40302) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40302
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "拜访";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00040302_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "能认识你这样年轻有潜力的勇士，我也很荣幸，虽然我是莫名其妙的来到这里，不知道你是做什么的，但是帝释天向我保证，所有的天族勇士都是了不起的人。";
	action.m_ActionMsg = "嗯王说的没有错。";
	return action;
end

function Task_00040302_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "哈哈你这年轻人还真是不谦虚，据说名将伍子胥也来到你们天庭了，你帮我拜访他一下吧，他好像在南边的瞻部峰那里。";
	action.m_ActionMsg = "似乎听说过这样的事情，我会帮你的。";
	return action;
end

function Task_00040302_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "居然是孙膑给我写的信，我来看看。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00040302_step_table = {
		[1] = Task_00040302_step_01,
		[2] = Task_00040302_step_02,
		[10] = Task_00040302_step_10,
		};

function Task_00040302_step(step)
	if Task_00040302_step_table[step] ~= nil then
		return Task_00040302_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00040302_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040302() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(40302) then
		return false;
	end
	task:AddTaskStep(40302);
	return true;
end



--�ύ����
function Task_00040302_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8911,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(40302) then
		return false;
	end

	if IsEquipTypeId(8911) then
		for k = 1, 1 do
			package:AddEquip(8911, 1);
		end
	else 
		package:AddItem(8911,1,1);
	end

	player:AddExp(1000);
	player:getCoin(900);
	return true;
end

--��������
function Task_00040302_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(40302);
end
