--����Ľ�������
function Task_Accept_00061301()
	local player = GetPlayer();
	if player:GetLev() < 53 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(61301) or task:HasCompletedTask(61301) or task:HasSubmitedTask(61301) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00061301()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 53 then
		return false;
	end
	if task:HasAcceptedTask(61301) or task:HasCompletedTask(61301) or task:HasSubmitedTask(61301) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00061301()
	if GetPlayer():GetTaskMgr():HasCompletedTask(61301) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00061301(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(61301) == npcId and Task_Accept_00061301 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 61301
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "收拾渔人";
	elseif task:GetTaskSubmitNpc(61301) == npcId then
		if Task_Submit_00061301() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61301
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "收拾渔人";
		elseif task:HasAcceptedTask(61301) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61301
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "收拾渔人";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00061301_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "说勇士勇士就到啊，我刚还在念叨呢，能不能来个勇士帮我把那些渔人赶走，你就来了。";
	action.m_ActionMsg = "那些渔人怎么你了？";
	return action;
end

function Task_00061301_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我正忙着制作各种药方呢，我旁边那些咸水岸的怒目渔人，就一直来打扰我工作，你能不能去帮我吧他们解决掉啊。";
	action.m_ActionMsg = "嗯没问题。";
	return action;
end

function Task_00061301_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "现在好多了，我可以安心制作我的药方了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00061301_step_table = {
		[1] = Task_00061301_step_01,
		[2] = Task_00061301_step_02,
		[10] = Task_00061301_step_10,
		};

function Task_00061301_step(step)
	if Task_00061301_step_table[step] ~= nil then
		return Task_00061301_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00061301_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00061301() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(61301) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00061301_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(61301) then
		return false;
	end


	player:AddExp(68000);
	player:getCoin(57000);
	return true;
end

--��������
function Task_00061301_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(61301);
end
