--����Ľ�������
function Task_Accept_00060801()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 46 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(60801) or task:HasCompletedTask(60801) or task:HasSubmitedTask(60801) then
		return false;
	end
	if not task:HasSubmitedTask(60703) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00060801()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 46 then
		return false;
	end
	if task:HasAcceptedTask(60801) or task:HasCompletedTask(60801) or task:HasSubmitedTask(60801) then
		return false;
	end
	if not task:HasSubmitedTask(60703) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00060801()
	if GetPlayer():GetTaskMgr():HasCompletedTask(60801) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00060801(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(60801) == npcId and Task_Accept_00060801 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 60801
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "求医问药";
	elseif task:GetTaskSubmitNpc(60801) == npcId then
		if Task_Submit_00060801() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60801
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "求医问药";
		elseif task:HasAcceptedTask(60801) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60801
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "求医问药";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00060801_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "既然盛一刀说治好失语症是送回紫衣姑娘唯一的条件，那现在必须找最好的大师求一个能治好紫衣姑娘的方子，你是否认识你们族内最好的大师呢？";
	action.m_ActionMsg = "我知道天庭有一个很厉害的炼丹大师神指。";
	return action;
end

function Task_00060801_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "那就拜托你去找这位大师求一个治疗失语症的方子了，作为感谢，把我近年来收藏的这些稀世草药带去吧，炼丹师都喜欢这些东西的。";
	action.m_ActionMsg = "好的，那我去给你问问。";
	return action;
end

function Task_00060801_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士又见面了啊，你又遇到什么麻烦事了吗？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00060801_step_table = {
		[1] = Task_00060801_step_01,
		[2] = Task_00060801_step_02,
		[10] = Task_00060801_step_10,
		};

function Task_00060801_step(step)
	if Task_00060801_step_table[step] ~= nil then
		return Task_00060801_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00060801_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00060801() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15008, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(60801) then
		return false;
	end
	task:AddTaskStep(60801);
	return true;
end



--�ύ����
function Task_00060801_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15008,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(60801) then
		return false;
	end

	package:DelItemAll(15008,1);

	player:AddExp(40000);
	player:getCoin(27000);
	return true;
end

--��������
function Task_00060801_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15008,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(60801);
end
