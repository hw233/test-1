--����Ľ�������
function Task_Accept_00031504()
	local player = GetPlayer();
	if player:GetLev() < 65 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31504) or task:HasCompletedTask(31504) or task:HasSubmitedTask(31504) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31503) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31503) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031504()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 65 then
		return false;
	end
	if task:HasAcceptedTask(31504) or task:HasCompletedTask(31504) or task:HasSubmitedTask(31504) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31503) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31503) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00031504()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31504) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031504(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31504) == npcId and Task_Accept_00031504 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31504
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "夜摩太师的覆没";
	elseif task:GetTaskSubmitNpc(31504) == npcId then
		if Task_Submit_00031504() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31504
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "夜摩太师的覆没";
		elseif task:HasAcceptedTask(31504) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31504
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "夜摩太师的覆没";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031504_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士，我知道你来找我是出于好心，虽然我目前是效忠夜摩盟的，但是我想 我应该陪你去一趟延维林看看，你会有所收获的，那里是夜摩盟的太师摩柯俱的藏身之地。";
	action.m_ActionMsg = "原来是这样。";
	return action;
end

function Task_00031504_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "是的，这位摩柯俱就是一直在背后帮助夜叉王和摩可拿出各种阴谋诡计的人，也是夜摩盟很多罪恶阴谋的来源。";
	action.m_ActionMsg = "那我们现在就去吧。";
	return action;
end

function Task_00031504_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "无奈我英明一世，却要以叛徒的命运收场啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031504_step_table = {
		[1] = Task_00031504_step_01,
		[2] = Task_00031504_step_02,
		[10] = Task_00031504_step_10,
		};

function Task_00031504_step(step)
	if Task_00031504_step_table[step] ~= nil then
		return Task_00031504_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031504_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031504() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31504) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00031504_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(1305,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(31504) then
		return false;
	end

	if IsEquipTypeId(1305) then
		for k = 1, 1 do
			package:AddEquip(1305, 1);
		end
	else 
		package:AddItem(1305,1,1);
	end

	player:AddExp(180000);
	player:getCoin(156000);
	player:getTael(40);
	return true;
end

--��������
function Task_00031504_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31504);
end
