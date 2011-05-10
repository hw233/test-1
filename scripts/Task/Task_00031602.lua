--����Ľ�������
function Task_Accept_00031602()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 67 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31602) or task:HasCompletedTask(31602) or task:HasSubmitedTask(31602) then
		return false;
	end
	if not task:HasSubmitedTask(31601) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031602()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 67 then
		return false;
	end
	if task:HasAcceptedTask(31602) or task:HasCompletedTask(31602) or task:HasSubmitedTask(31602) then
		return false;
	end
	if not task:HasSubmitedTask(31601) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00031602()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31602) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031602(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31602) == npcId and Task_Accept_00031602 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31602
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "惊动魔族";
	elseif task:GetTaskSubmitNpc(31602) == npcId then
		if Task_Submit_00031602() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31602
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "惊动魔族";
		elseif task:HasAcceptedTask(31602) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31602
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "惊动魔族";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031602_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士"..GetPlayerName(GetPlayer()).."你终于来了，你可知道你做了什么不可饶恕的事情？吕布这样难得一见的名将居然被你杀死了，我长期驻扎在这环境险恶的山海大陆，就是为了能为我们魔族招募这样一位难得一见的名将，可是你居然就为了逞英雄只能将他杀死了。";
	action.m_ActionMsg = "你最好查清楚，是他自己借我的力量自杀的。";
	return action;
end

function Task_00031602_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "可是他好好的为什么要自杀呢？";
	action.m_ActionMsg = "吕布自己后悔投靠夜摩盟，但是又不愿背负背叛的罪名，才这样做的。";
	return action;
end

function Task_00031602_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "怎么会有这样的事情发生？就算你这么说，怎么能让我相信呢！";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031602_step_table = {
		[1] = Task_00031602_step_01,
		[2] = Task_00031602_step_02,
		[10] = Task_00031602_step_10,
		};

function Task_00031602_step(step)
	if Task_00031602_step_table[step] ~= nil then
		return Task_00031602_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031602_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031602() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31602) then
		return false;
	end
	task:AddTaskStep(31602);
	return true;
end



--�ύ����
function Task_00031602_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8912,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(31602) then
		return false;
	end

	if IsEquipTypeId(8912) then
		for k = 1, 1 do
			package:AddEquip(8912, 1);
		end
	else 
		package:AddItem(8912,1,1);
	end

	player:AddExp(200000);
	player:getCoin(180000);
	player:getTael(50);
	return true;
end

--��������
function Task_00031602_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31602);
end
