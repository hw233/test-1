--����Ľ�������
function Task_Accept_00020104()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 5 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(20104) or task:HasCompletedTask(20104) or task:HasSubmitedTask(20104) then
		return false;
	end
	if not task:HasSubmitedTask(20103) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00020104()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 5 then
		return false;
	end
	if task:HasAcceptedTask(20104) or task:HasCompletedTask(20104) or task:HasSubmitedTask(20104) then
		return false;
	end
	if not task:HasSubmitedTask(20103) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00020104()
	if GetPlayer():GetTaskMgr():HasCompletedTask(20104) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00020104(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(20104) == npcId and Task_Accept_00020104 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 20104
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "危机四伏";
	elseif task:GetTaskSubmitNpc(20104) == npcId then
		if Task_Submit_00020104() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20104
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "危机四伏";
		elseif task:HasAcceptedTask(20104) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20104
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "危机四伏";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00020104_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "最近一直听人禀报说十宝山的野岭非常乱，他们的生活受到了很多怪物的骚扰，族民们痛苦不堪，我们的火神毗摩质多罗肯定有要事缠身忙不过来，你去查看一下吧。";
	action.m_ActionMsg = "居然有怪物骚扰我们的族民，我要去查个清楚。";
	return action;
end

function Task_00020104_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "查出有任何结果以后，去毗摩宫禀报给火神毗摩质多罗。";
	action.m_ActionMsg = "没问题，我会很好的完成我的任务。";
	return action;
end

function Task_00020104_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "原来是这些邪恶的家伙，幸好你替我发现了他们。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00020104_step_table = {
		[1] = Task_00020104_step_01,
		[2] = Task_00020104_step_02,
		[10] = Task_00020104_step_10,
		};

function Task_00020104_step(step)
	if Task_00020104_step_table[step] ~= nil then
		return Task_00020104_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00020104_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00020104() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(20104) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00020104_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8908,1,1) + package:GetItemUsedGrids(4,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(20104) then
		return false;
	end

	if IsEquipTypeId(8908) then
		for k = 1, 1 do
			package:AddEquip(8908, 1);
		end
	else 
		package:AddItem(8908,1,1);
	end
	if IsEquipTypeId(4) then
		for k = 1, 1 do
			package:AddEquip(4, 1);
		end
	else 
		package:AddItem(4,1,1);
	end

	player:AddExp(700);
	player:getCoin(650);
	return true;
end

--��������
function Task_00020104_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(20104);
end
