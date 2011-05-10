--����Ľ�������
function Task_Accept_00031805()
	local player = GetPlayer();
	if player:GetLev() < 69 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31805) or task:HasCompletedTask(31805) or task:HasSubmitedTask(31805) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31804) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31804) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031805()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 69 then
		return false;
	end
	if task:HasAcceptedTask(31805) or task:HasCompletedTask(31805) or task:HasSubmitedTask(31805) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31804) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31804) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00031805()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31805) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031805(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31805) == npcId and Task_Accept_00031805 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31805
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "额提的代价";
	elseif task:GetTaskSubmitNpc(31805) == npcId then
		if Task_Submit_00031805() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31805
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "额提的代价";
		elseif task:HasAcceptedTask(31805) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31805
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "额提的代价";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031805_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士啊，严中的女儿也送走了，现在容我跟你说几句话吧。";
	action.m_ActionMsg = "你说吧，我在听。";
	return action;
end

function Task_00031805_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "其实就算你不来找我，我也想到会有这样一天，虽然我因为贪财背叛了镇邪派，背叛了祖言帮主，这的确让我无颜以对，可是在夜摩盟，我也很快得到我想要的金钱和地位，却失去了祖言帮主那样不屈的信念，夜摩盟就是一边觊觎天魔的势力，一边又在做些鸡鸣狗盗的事情，我再也不想这样下去了。";
	action.m_ActionMsg = "那你想怎样呢？";
	return action;
end

function Task_00031805_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士太感谢你了，既解决了镇邪派的叛徒，又救回了我的女儿。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031805_step_table = {
		[1] = Task_00031805_step_01,
		[2] = Task_00031805_step_02,
		[10] = Task_00031805_step_10,
		};

function Task_00031805_step(step)
	if Task_00031805_step_table[step] ~= nil then
		return Task_00031805_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031805_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031805() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31805) then
		return false;
	end
	RunAutoBattleAction(player,4296,100);
	task:AddTaskStep(31805);

	return true;
end



--�ύ����
function Task_00031805_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(5043,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(31805) then
		return false;
	end

	if IsEquipTypeId(5043) then
		for k = 1, 1 do
			package:AddEquip(5043, 1);
		end
	else 
		package:AddItem(5043,1,1);
	end

	player:AddExp(220000);
	player:getCoin(204000);
	player:getTael(40);
	return true;
end

--��������
function Task_00031805_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31805);
end
