--����Ľ�������
function Task_Accept_00062004()
	local player = GetPlayer();
	if player:GetLev() < 71 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(62004) or task:HasCompletedTask(62004) or task:HasSubmitedTask(62004) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(62003) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(62003) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00062004()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 71 then
		return false;
	end
	if task:HasAcceptedTask(62004) or task:HasCompletedTask(62004) or task:HasSubmitedTask(62004) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(62003) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(62003) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00062004()
	if GetPlayer():GetTaskMgr():HasCompletedTask(62004) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00062004(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(62004) == npcId and Task_Accept_00062004 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 62004
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "自主信念";
	elseif task:GetTaskSubmitNpc(62004) == npcId then
		if Task_Submit_00062004() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62004
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "自主信念";
		elseif task:HasAcceptedTask(62004) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62004
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "自主信念";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00062004_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "生亦何欢，死亦何苦，求中下签，卜中上卦。";
	action.m_ActionMsg = "大师，此话怎讲？";
	return action;
end

function Task_00062004_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "影响他心智的怪物也已经被杀掉了，对前途的判断并不是算命算出来的，而是用靠自己的信念而选择。中下签虽然命运多坎坷，可是这样的命运往往预示着中上卦，未来更容易由自己来选择和掌控。";
	action.m_ActionMsg = "还是没有说到底算了些什么嘛。";
	return action;
end

function Task_00062004_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "大师的占卜果然高深啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00062004_step_table = {
		[1] = Task_00062004_step_01,
		[2] = Task_00062004_step_02,
		[10] = Task_00062004_step_10,
		};

function Task_00062004_step(step)
	if Task_00062004_step_table[step] ~= nil then
		return Task_00062004_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00062004_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00062004() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(62004) then
		return false;
	end
	task:AddTaskStep(62004);
	return true;
end



--�ύ����
function Task_00062004_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8915,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(62004) then
		return false;
	end

	if IsEquipTypeId(8915) then
		for k = 1, 1 do
			package:AddEquip(8915, 1);
		end
	else 
		package:AddItem(8915,1,1);
	end

	player:AddExp(250000);
	player:getCoin(230000);
	return true;
end

--��������
function Task_00062004_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(62004);
end
