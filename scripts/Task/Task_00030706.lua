--����Ľ�������
function Task_Accept_00030706()
	local player = GetPlayer();
	if player:GetLev() < 49 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30706) or task:HasCompletedTask(30706) or task:HasSubmitedTask(30706) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30705) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30705) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030706()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 49 then
		return false;
	end
	if task:HasAcceptedTask(30706) or task:HasCompletedTask(30706) or task:HasSubmitedTask(30706) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30705) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30705) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030706()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30706) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030706(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30706) == npcId and Task_Accept_00030706 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30706
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "黎朝国的历史";
	elseif task:GetTaskSubmitNpc(30706) == npcId then
		if Task_Submit_00030706() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30706
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "黎朝国的历史";
		elseif task:HasAcceptedTask(30706) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30706
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "黎朝国的历史";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030706_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士你好，很高兴你能来帮助我们，不过看你一脸的尴尬，是不是被我们的首领祖言冷冰冰的态度吓到了。";
	action.m_ActionMsg = "是啊，没见过态度这么冷的首领。";
	return action;
end

function Task_00030706_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "希望你不要太介意，其实接触多了，你就会知道我们的首领祖言是很了不起的。让我讲给你听吧，我们的首领还是小孩子的时候，整个黎朝国受到了摩可拿的袭击而频临灭亡，祖言是黎朝国皇室唯一的幸存者，在他的带领下，才有了今天的镇邪派。";
	action.m_ActionMsg = "原来祖言帮主有这样的身世。";
	return action;
end

function Task_00030706_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "不要担心祖言帮主的态度，他其实是一个责任心很强的人。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030706_step_table = {
		[1] = Task_00030706_step_01,
		[2] = Task_00030706_step_02,
		[10] = Task_00030706_step_10,
		};

function Task_00030706_step(step)
	if Task_00030706_step_table[step] ~= nil then
		return Task_00030706_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030706_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030706() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30706) then
		return false;
	end
	task:AddTaskStep(30706);
	return true;
end



--�ύ����
function Task_00030706_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(307,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(30706) then
		return false;
	end

	if IsEquipTypeId(307) then
		for k = 1, 1 do
			package:AddEquip(307, 1);
		end
	else 
		package:AddItem(307,1,1);
	end

	player:AddExp(52000);
	player:getCoin(40000);
	player:getTael(30);
	return true;
end

--��������
function Task_00030706_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30706);
end
