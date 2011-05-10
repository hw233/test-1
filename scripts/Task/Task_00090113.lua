--����Ľ�������
function Task_Accept_00090113()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90113) or task:HasCompletedTask(90113) or task:HasSubmitedTask(90113) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00090113()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90113) or task:HasCompletedTask(90113) or task:HasSubmitedTask(90113) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00090113()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90113) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00090113(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90113) == npcId and Task_Accept_00090113 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90113
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "夜摩国师";
	elseif task:GetTaskSubmitNpc(90113) == npcId then
		if Task_Submit_00090113() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90113
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "夜摩国师";
		elseif task:HasAcceptedTask(90113) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90113
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "夜摩国师";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00090113_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "既然吕布不愿意来那你就帮我个忙吧。";
	action.m_ActionMsg = "将军请讲。";
	return action;
end

function Task_00090113_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "在离这不远的玄幻坡驻扎着夜摩国师摩柯心，他是摩柯俱的兄弟。你去杀了摩柯心，至于摩柯俱，有吕布在那看着，不足为患。 ";
	action.m_ActionMsg = "将军且等我的好消息。";
	return action;
end

function Task_00090113_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "但愿这次你是真将他杀了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090113_step_table = {
		[1] = Task_00090113_step_01,
		[2] = Task_00090113_step_02,
		[10] = Task_00090113_step_10,
		};

function Task_00090113_step(step)
	if Task_00090113_step_table[step] ~= nil then
		return Task_00090113_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00090113_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090113() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90113) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00090113_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(90113) then
		return false;
	end


	player:AddExp(625000);
	player:getCoin(310000);
	return true;
end

--��������
function Task_00090113_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90113);
end
