--����Ľ�������
function Task_Accept_00061302()
	local player = GetPlayer();
	if player:GetLev() < 53 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(61302) or task:HasCompletedTask(61302) or task:HasSubmitedTask(61302) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61301) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61301) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00061302()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 53 then
		return false;
	end
	if task:HasAcceptedTask(61302) or task:HasCompletedTask(61302) or task:HasSubmitedTask(61302) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61301) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61301) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00061302()
	if GetPlayer():GetTaskMgr():HasCompletedTask(61302) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00061302(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(61302) == npcId and Task_Accept_00061302 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 61302
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "创伤药";
	elseif task:GetTaskSubmitNpc(61302) == npcId then
		if Task_Submit_00061302() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61302
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "创伤药";
		elseif task:HasAcceptedTask(61302) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61302
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "创伤药";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00061302_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "我趁着最近闲暇的时候，收集了一些草药，制作了一批创伤药，我知道山海大陆上的镇邪派是一个正直而顽强的团体，可是却不那么的强势和富裕，我想这些创伤药对他们会有一些帮助。";
	action.m_ActionMsg = "医官您真是料事如神啊，镇邪派的确是很需要资助。";
	return action;
end

function Task_00061302_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "因为我知道他们的一些卫兵，在与狼鬼的抗争中受了伤，却经常因为缺少医疗条件，而不能及时医治。事不宜迟，你快就帮我把这些创伤药送到响石滩的黎明镇，给他们的帮主祖言吧。";
	action.m_ActionMsg = "医官，您真是个好人，他们的族人会感谢您的，";
	return action;
end

function Task_00061302_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "天下居然有这么好的医官。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00061302_step_table = {
		[1] = Task_00061302_step_01,
		[2] = Task_00061302_step_02,
		[10] = Task_00061302_step_10,
		};

function Task_00061302_step(step)
	if Task_00061302_step_table[step] ~= nil then
		return Task_00061302_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00061302_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00061302() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15012, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(61302) then
		return false;
	end
	task:AddTaskStep(61302);
	return true;
end



--�ύ����
function Task_00061302_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15012,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(61302) then
		return false;
	end

	package:DelItemAll(15012,1);

	player:AddExp(68000);
	player:getCoin(57000);
	return true;
end

--��������
function Task_00061302_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15012,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(61302);
end
