--����Ľ�������
function Task_Accept_00061603()
	local player = GetPlayer();
	if player:GetLev() < 61 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(61603) or task:HasCompletedTask(61603) or task:HasSubmitedTask(61603) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61602) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61602) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00061603()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 61 then
		return false;
	end
	if task:HasAcceptedTask(61603) or task:HasCompletedTask(61603) or task:HasSubmitedTask(61603) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61602) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61602) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00061603()
	if GetPlayer():GetTaskMgr():HasCompletedTask(61603) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00061603(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(61603) == npcId and Task_Accept_00061603 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 61603
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "嫁祸他人";
	elseif task:GetTaskSubmitNpc(61603) == npcId then
		if Task_Submit_00061603() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61603
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "嫁祸他人";
		elseif task:HasAcceptedTask(61603) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61603
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "嫁祸他人";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00061603_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士，你不能只看到这小册子上写的是我的名字，就认定是我写的，也许我是被迫写的呢？";
	action.m_ActionMsg = "我才不会信你。";
	return action;
end

function Task_00061603_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "其实我真的是被迫写的，如果不信你可以去雪崖峰上看，那里的绝尘冰妖无比的厉害，他们才是真正的罪魁祸首，我也是受到了他们的威胁，可是我又无力对抗，才被迫写了这样的东西，他们才是目的邪恶的，你去那里把他们打败就不会再有这样的事情了。";
	action.m_ActionMsg = "我权且去见识一下那是些什么妖怪。";
	return action;
end

function Task_00061603_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你怎么会这么快就把他们打败了？难道他们不是非常厉害的吗？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00061603_step_table = {
		[1] = Task_00061603_step_01,
		[2] = Task_00061603_step_02,
		[10] = Task_00061603_step_10,
		};

function Task_00061603_step(step)
	if Task_00061603_step_table[step] ~= nil then
		return Task_00061603_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00061603_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00061603() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(61603) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00061603_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8906,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(61603) then
		return false;
	end

	if IsEquipTypeId(8906) then
		for k = 1, 1 do
			package:AddEquip(8906, 1);
		end
	else 
		package:AddItem(8906,1,1);
	end

	player:AddExp(135000);
	player:getCoin(115000);
	return true;
end

--��������
function Task_00061603_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(61603);
end
