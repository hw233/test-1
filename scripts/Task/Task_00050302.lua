--����Ľ�������
function Task_Accept_00050302()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 10 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(50302) or task:HasCompletedTask(50302) or task:HasSubmitedTask(50302) then
		return false;
	end
	if not task:HasSubmitedTask(50301) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00050302()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 10 then
		return false;
	end
	if task:HasAcceptedTask(50302) or task:HasCompletedTask(50302) or task:HasSubmitedTask(50302) then
		return false;
	end
	if not task:HasSubmitedTask(50301) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00050302()
	if GetPlayer():GetTaskMgr():HasCompletedTask(50302) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00050302(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(50302) == npcId and Task_Accept_00050302 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50302
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "摆脱污言";
	elseif task:GetTaskSubmitNpc(50302) == npcId then
		if Task_Submit_00050302() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50302
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "摆脱污言";
		elseif task:HasAcceptedTask(50302) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50302
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "摆脱污言";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00050302_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "我说这位勇士，庞涓的话你也信，你对自己也太没信心了，我只是看不起有些懒惰的修道者，然而像你这样的有潜力又够努力的修道者，我佩服还来不及呢。";
	action.m_ActionMsg = "这个……";
	return action;
end

function Task_00050302_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "如果不信你可以去波斯遗址杀掉那里的疯狂盗猎者，如果你能打败他们，就说明你很厉害了，因为无数懒惰的修道者已经败在那些怪物的手里。";
	action.m_ActionMsg = "我显然不是那样的人，你等着。";
	return action;
end

function Task_00050302_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我就说吧，懒惰的人去了那里就回不来了，庞涓的话你还是别太当真了，他有的时候就是心里不平衡。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00050302_step_table = {
		[1] = Task_00050302_step_01,
		[2] = Task_00050302_step_02,
		[10] = Task_00050302_step_10,
		};

function Task_00050302_step(step)
	if Task_00050302_step_table[step] ~= nil then
		return Task_00050302_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00050302_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00050302() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(50302) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00050302_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(50302) then
		return false;
	end


	player:AddExp(1000);
	player:getCoin(900);
	return true;
end

--��������
function Task_00050302_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(50302);
end
