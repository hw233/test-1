--����Ľ�������
function Task_Accept_00032001()
	local player = GetPlayer();
	if player:GetLev() < 74 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(32001) or task:HasCompletedTask(32001) or task:HasSubmitedTask(32001) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31905) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31905) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00032001()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 74 then
		return false;
	end
	if task:HasAcceptedTask(32001) or task:HasCompletedTask(32001) or task:HasSubmitedTask(32001) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31905) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31905) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00032001()
	if GetPlayer():GetTaskMgr():HasCompletedTask(32001) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00032001(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(32001) == npcId and Task_Accept_00032001 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 32001
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "阴谋暴露";
	elseif task:GetTaskSubmitNpc(32001) == npcId then
		if Task_Submit_00032001() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 32001
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "阴谋暴露";
		elseif task:HasAcceptedTask(32001) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 32001
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "阴谋暴露";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00032001_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士很高兴你成长的这么快，已经穿过各种险恶的地势，来到这山海大陆上最危险的地方，没有足够的实力，是没有办法在这里生存下去的，我们东山派也是只有最尖锐的部队才会驻扎在这样的地方。";
	action.m_ActionMsg = "哦，你好。";
	return action;
end

function Task_00032001_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我们的部队，就快要弄清楚夜摩盟的确切驻地和详细阴谋了，如果你能加入我们，对我们来说更是如虎添翼，最好你能现在就帮我们去灰岩塔秘密探查一下，看看他们在密谋什么。";
	action.m_ActionMsg = "没问题，我去看看。";
	return action;
end

function Task_00032001_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "有你这样的勇士加入我们，一定可以更好的实现我们阻止夜摩盟势力。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00032001_step_table = {
		[1] = Task_00032001_step_01,
		[2] = Task_00032001_step_02,
		[10] = Task_00032001_step_10,
		};

function Task_00032001_step(step)
	if Task_00032001_step_table[step] ~= nil then
		return Task_00032001_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00032001_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00032001() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(32001) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00032001_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8923,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(32001) then
		return false;
	end

	if IsEquipTypeId(8923) then
		for k = 1, 1 do
			package:AddEquip(8923, 1);
		end
	else 
		package:AddItem(8923,1,1);
	end

	player:AddExp(290000);
	player:getCoin(275000);
	player:getTael(40);
	return true;
end

--��������
function Task_00032001_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(32001);
end
