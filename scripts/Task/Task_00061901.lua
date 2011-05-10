--����Ľ�������
function Task_Accept_00061901()
	local player = GetPlayer();
	if player:GetLev() < 68 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(61901) or task:HasCompletedTask(61901) or task:HasSubmitedTask(61901) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00061901()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 68 then
		return false;
	end
	if task:HasAcceptedTask(61901) or task:HasCompletedTask(61901) or task:HasSubmitedTask(61901) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00061901()
	if GetPlayer():GetTaskMgr():HasCompletedTask(61901) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00061901(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(61901) == npcId and Task_Accept_00061901 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 61901
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "星象不稳";
	elseif task:GetTaskSubmitNpc(61901) == npcId then
		if Task_Submit_00061901() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61901
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "星象不稳";
		elseif task:HasAcceptedTask(61901) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61901
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "星象不稳";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00061901_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士我最近有了一个很大的发现，正想找人说一下，我长期在鹿园潜心观天象，钻研日月星的运行轨迹以及预示的各种大事件，就在前几日，我发现星象及其不稳，忽明忽暗且轨迹杂乱，似乎世界末日将至啊。";
	action.m_ActionMsg = "这不是真的吧，你对这研究结果有几分的把握？";
	return action;
end

function Task_00061901_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "虽然我经常被认为是个星象疯子，可是只要这个现象没有错，我的预言就肯定是准确的。不然你去常泉坡的玉虚道场，问一下那里的善觉长者，他是个理学大师，自认科学客观，问问他如果真的是世界末日了，要怎么做才好。";
	action.m_ActionMsg = "难道真有世界末日这一说吗？不会吧。";
	return action;
end

function Task_00061901_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "无影又跟你说了什么神乎其神的预言了？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00061901_step_table = {
		[1] = Task_00061901_step_01,
		[2] = Task_00061901_step_02,
		[10] = Task_00061901_step_10,
		};

function Task_00061901_step(step)
	if Task_00061901_step_table[step] ~= nil then
		return Task_00061901_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00061901_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00061901() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(61901) then
		return false;
	end
	task:AddTaskStep(61901);
	return true;
end



--�ύ����
function Task_00061901_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(61901) then
		return false;
	end


	player:AddExp(210000);
	player:getCoin(190000);
	return true;
end

--��������
function Task_00061901_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(61901);
end
