--����Ľ�������
function Task_Accept_00090114()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90114) or task:HasCompletedTask(90114) or task:HasSubmitedTask(90114) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00090114()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90114) or task:HasCompletedTask(90114) or task:HasSubmitedTask(90114) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00090114()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90114) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00090114(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90114) == npcId and Task_Accept_00090114 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90114
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "秘制美酒";
	elseif task:GetTaskSubmitNpc(90114) == npcId then
		if Task_Submit_00090114() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90114
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "秘制美酒";
		elseif task:HasAcceptedTask(90114) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90114
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "秘制美酒";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00090114_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，我听说你是夜摩城来的。那想必你对那很熟悉了，黑风寨你应该知道，那里住着一群可恶的强盗。他们路过我这的时候居然抢走了我亲手酿制的美酒，你能帮我抢回来吗？";
	action.m_ActionMsg = "美酒？这会肯定被那些怪物喝没了，我尽量给你抢回一些。";
	return action;
end

function Task_00090114_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "真是可惜，只剩这一坛了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090114_step_table = {
		[1] = Task_00090114_step_01,
		[10] = Task_00090114_step_10,
		};

function Task_00090114_step(step)
	if Task_00090114_step_table[step] ~= nil then
		return Task_00090114_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00090114_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090114() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90114) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00090114_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15105,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(90114) then
		return false;
	end

	package:DelItemAll(15105,1);

	player:AddExp(625000);
	player:getCoin(310000);
	return true;
end

--��������
function Task_00090114_abandon()
	local package = GetPlayer():GetPackage();
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90114);
end
