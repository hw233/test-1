--����Ľ�������
function Task_Accept_00040802()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 25 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40802) or task:HasCompletedTask(40802) or task:HasSubmitedTask(40802) then
		return false;
	end
	if not task:HasSubmitedTask(40801) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00040802()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 25 then
		return false;
	end
	if task:HasAcceptedTask(40802) or task:HasCompletedTask(40802) or task:HasSubmitedTask(40802) then
		return false;
	end
	if not task:HasSubmitedTask(40801) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00040802()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40802) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00040802(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40802) == npcId and Task_Accept_00040802 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40802
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "救出战马";
	elseif task:GetTaskSubmitNpc(40802) == npcId then
		if Task_Submit_00040802() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40802
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "救出战马";
		elseif task:HasAcceptedTask(40802) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40802
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "救出战马";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00040802_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "你也知道我不是非常擅长亲自上阵，可是我的心爱的战马在我上一次出门的时候走失了，我找了好久没有找到，一直很担心，有族民告诉我他们在绝世林附近发现了一匹马，直觉告诉我那应该就是我的战马。";
	action.m_ActionMsg = "那你是让我帮你去看看吗？";
	return action;
end

function Task_00040802_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我正是想拜托你去帮我看看，我想一定是因为那里有什么怪物困住了我的马了，如果你能杀掉那些邪恶的怪物，把我的马救回来就好了。";
	action.m_ActionMsg = "我会试试看。";
	return action;
end

function Task_00040802_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "救命。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00040802_step_table = {
		[1] = Task_00040802_step_01,
		[2] = Task_00040802_step_02,
		[10] = Task_00040802_step_10,
		};

function Task_00040802_step(step)
	if Task_00040802_step_table[step] ~= nil then
		return Task_00040802_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00040802_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040802() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(40802) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00040802_submit(itemId, itemNum)
	local player = GetPlayer();

	--���ѡ������Ʒ
	local select = false;
	if itemId == 190 and itemNum == 1 then
		select = true;
	elseif itemId == 191 and itemNum == 1 then
		select = true;
	elseif itemId == 194 and itemNum == 1 then
		select = true;
	end

	if not select then return false; end
	local package = player:GetPackage();

	local selReqGrid = package:GetItemUsedGrids(itemId, itemNum, 1);
	if selReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(40802) then
		return false;
	end

	if IsEquipTypeId(itemId) then 
		for j = 1, itemNum do
			package:AddEquip(itemId, 1);
		end
	else
		package:AddItem(itemId, itemNum, 1);
	end

	player:AddExp(3000);
	player:getCoin(4000);
	return true;
end

--��������
function Task_00040802_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(40802);
end
