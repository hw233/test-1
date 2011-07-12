--����Ľ�������
function Task_Accept_00000013()
	local player = GetPlayer();
	if player:GetLev() < 11 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(13) or task:HasCompletedTask(13) or task:HasSubmitedTask(13) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(12) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(12) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(12) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000013()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 11 then
		return false;
	end
	if task:HasAcceptedTask(13) or task:HasCompletedTask(13) or task:HasSubmitedTask(13) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(12) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(12) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(12) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000013()
	if GetPlayer():GetTaskMgr():HasCompletedTask(13) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000013(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(13) == npcId and Task_Accept_00000013 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 13
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "寻觅帮手";
	elseif task:GetTaskSubmitNpc(13) == npcId then
		if Task_Submit_00000013() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 13
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "寻觅帮手";
		elseif task:HasAcceptedTask(13) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 13
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "寻觅帮手";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000013_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).." , 你可知现在正值第三次峨眉斗剑，正邪都在为此做准备。各派人士四处寻觅根骨资质俱佳的弟子，你一人恐怕势单力薄，不如多结交一些同道中人。";
	action.m_ActionMsg = "我初来乍到，人地不熟，去哪里找是好呢？";
	return action;
end

function Task_00000013_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "呵呵，远在天边，近在眼前呀，"..GetPlayerName(GetPlayer()).."你刚刚打过交道的知味楼掌柜识人丰富，他一定能帮你介绍几个同道人士。";
	action.m_ActionMsg = "多谢前辈指教，我这就去看看。";
	return action;
end

function Task_00000013_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "少侠找我就对了，谁不知道方圆百里就属我的人脉最广了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000013_step_table = {
		[1] = Task_00000013_step_01,
		[2] = Task_00000013_step_02,
		[10] = Task_00000013_step_10,
		};

function Task_00000013_step(step)
	if Task_00000013_step_table[step] ~= nil then
		return Task_00000013_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000013_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000013() then
		return false;
	end
	if not task:AcceptTask(13) then
		return false;
	end
	task:AddTaskStep(13);
	return true;
end



--�ύ����
function Task_00000013_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(13) then
		return false;
	end


	player:AddExp(2000);
	player:getTael(222);
	return true;
end

--��������
function Task_00000013_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(13);
end
