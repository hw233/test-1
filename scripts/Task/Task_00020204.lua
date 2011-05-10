--����Ľ�������
function Task_Accept_00020204()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 14 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(20204) or task:HasCompletedTask(20204) or task:HasSubmitedTask(20204) then
		return false;
	end
	if not task:HasSubmitedTask(20203) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00020204()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 14 then
		return false;
	end
	if task:HasAcceptedTask(20204) or task:HasCompletedTask(20204) or task:HasSubmitedTask(20204) then
		return false;
	end
	if not task:HasSubmitedTask(20203) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00020204()
	if GetPlayer():GetTaskMgr():HasCompletedTask(20204) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00020204(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(20204) == npcId and Task_Accept_00020204 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 20204
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "震怒的火神";
	elseif task:GetTaskSubmitNpc(20204) == npcId then
		if Task_Submit_00020204() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20204
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "震怒的火神";
		elseif task:HasAcceptedTask(20204) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20204
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "震怒的火神";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00020204_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "（看完信）这个自私狂妄的帝释天，当初我正在跟阿修罗讨论事情，他就冒然派个乐师来提亲，想要娶我的女儿悦意，我和阿修罗怎么可能就这么轻易的让他得逞呢！";
	action.m_ActionMsg = "那后来他为什么还是娶走了公主呢？";
	return action;
end

function Task_00020204_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "阿修罗也对帝释天恨之入骨，于是带领我们的魔族跟天族发起了战争，我们相遇在阿兰若山谷，战争打了很久很久，我们的势力受到了很大的打击，阿修罗可能觉得战争拖的越久，拥有甘露的天族就越有优势，而悦意看到战死的魔族族众越来越多，甚至因达罗也受了伤，实在不愿意看到更惨烈的结果，就毅然提出以自己为条件来结束战争，嫁给了帝释天，作为讲和条件，天庭给我们供应我们很需要的甘露。";
	action.m_ActionMsg = "公主的精神真的很让人感动，我想去见见战神因达罗。";
	return action;
end

function Task_00020204_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你已经见过毗摩质多罗了吧。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00020204_step_table = {
		[1] = Task_00020204_step_01,
		[2] = Task_00020204_step_02,
		[10] = Task_00020204_step_10,
		};

function Task_00020204_step(step)
	if Task_00020204_step_table[step] ~= nil then
		return Task_00020204_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00020204_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00020204() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(20204) then
		return false;
	end
	task:AddTaskStep(20204);
	return true;
end



--�ύ����
function Task_00020204_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8917,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(20204) then
		return false;
	end

	if IsEquipTypeId(8917) then
		for k = 1, 1 do
			package:AddEquip(8917, 1);
		end
	else 
		package:AddItem(8917,1,1);
	end

	player:AddExp(1400);
	player:getCoin(1200);
	player:getTael(10);
	return true;
end

--��������
function Task_00020204_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(20204);
end
