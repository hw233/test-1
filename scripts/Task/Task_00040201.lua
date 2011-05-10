--����Ľ�������
function Task_Accept_00040201()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40201) or task:HasCompletedTask(40201) or task:HasSubmitedTask(40201) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00040201()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(40201) or task:HasCompletedTask(40201) or task:HasSubmitedTask(40201) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00040201()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40201) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00040201(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40201) == npcId and Task_Accept_00040201 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40201
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "跑腿";
	elseif task:GetTaskSubmitNpc(40201) == npcId then
		if Task_Submit_00040201() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40201
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "跑腿";
		elseif task:HasAcceptedTask(40201) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40201
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "跑腿";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00040201_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士，不知道你是否见过我们的摩伽天，他在瞻部峰的武都镇，掌管天族很多很重要的资源，如果你有兴趣，就去一趟代我问候一下他，问问题我们族内的情况是否一切顺利。";
	action.m_ActionMsg = "好的，那我就去问问看。";
	return action;
end

function Task_00040201_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "阿修罗也对帝释天恨之入骨，于是带领我们的魔族跟天族发起了战争，我们相遇在阿兰若山谷，战争打了很久很久，我们的势力受到了很大的打击，阿修罗可能觉得战争拖的越久，拥有甘露的天族就越有优势，而悦意看到战死的魔族族众越来越多，甚至因达罗也受了伤，实在不愿意看到更惨烈的结果，就毅然提出以自己为条件来结束战争。";
	action.m_ActionMsg = "";
	return action;
end

function Task_00040201_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "怎么会是悦意王妃让你来的?";
	action.m_ActionMsg = "";
	return action;
end

local Task_00040201_step_table = {
		[1] = Task_00040201_step_01,
		[2] = Task_00040201_step_02,
		[10] = Task_00040201_step_10,
		};

function Task_00040201_step(step)
	if Task_00040201_step_table[step] ~= nil then
		return Task_00040201_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00040201_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040201() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(40201) then
		return false;
	end
	task:AddTaskStep(40201);
	return true;
end



--�ύ����
function Task_00040201_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(40201) then
		return false;
	end


	player:AddExp(1000);
	player:getCoin(750);
	return true;
end

--��������
function Task_00040201_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(40201);
end
