--����Ľ�������
function Task_Accept_00040901()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 28 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40901) or task:HasCompletedTask(40901) or task:HasSubmitedTask(40901) then
		return false;
	end
	if not task:HasSubmitedTask(40803) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00040901()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 28 then
		return false;
	end
	if task:HasAcceptedTask(40901) or task:HasCompletedTask(40901) or task:HasSubmitedTask(40901) then
		return false;
	end
	if not task:HasSubmitedTask(40803) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00040901()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40901) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00040901(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40901) == npcId and Task_Accept_00040901 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40901
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "意气风发";
	elseif task:GetTaskSubmitNpc(40901) == npcId then
		if Task_Submit_00040901() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40901
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "意气风发";
		elseif task:HasAcceptedTask(40901) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40901
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "意气风发";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00040901_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士看你意气风发的样子，想必你在天庭的经历让你很满意啊，不过我想现在就告知你，我们的天庭出于对新手的保护，以强大的力量一直抵御我们的对立势力和一些邪恶势力对我们天族的威胁，可是如果将来你一旦离开这样的保护，你将不得不独自面临许多未知的危险。";
	action.m_ActionMsg = "这个我已经做好了心理准备。";
	return action;
end

function Task_00040901_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "既然你在心理上已经有所准备，我想你现在去观音殿找一下圣严，让他给你做一下战争的洗礼，让你将来在面对不同的抉择的时候更加勇敢和理智。";
	action.m_ActionMsg = "有必要一定要这样吗？！";
	return action;
end

function Task_00040901_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "天地无极，人海茫茫，接受洗礼，永生无畏。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00040901_step_table = {
		[1] = Task_00040901_step_01,
		[2] = Task_00040901_step_02,
		[10] = Task_00040901_step_10,
		};

function Task_00040901_step(step)
	if Task_00040901_step_table[step] ~= nil then
		return Task_00040901_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00040901_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040901() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(40901) then
		return false;
	end
	task:AddTaskStep(40901);
	return true;
end



--�ύ����
function Task_00040901_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(40901) then
		return false;
	end


	player:AddExp(4500);
	player:getCoin(4500);
	return true;
end

--��������
function Task_00040901_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(40901);
end
