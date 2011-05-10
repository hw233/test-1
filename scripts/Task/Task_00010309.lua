--����Ľ�������
function Task_Accept_00010309()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 18 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(10309) or task:HasCompletedTask(10309) or task:HasSubmitedTask(10309) then
		return false;
	end
	if not task:HasSubmitedTask(10308) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00010309()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 18 then
		return false;
	end
	if task:HasAcceptedTask(10309) or task:HasCompletedTask(10309) or task:HasSubmitedTask(10309) then
		return false;
	end
	if not task:HasSubmitedTask(10308) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00010309()
	if GetPlayer():GetTaskMgr():HasCompletedTask(10309) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00010309(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(10309) == npcId and Task_Accept_00010309 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 10309
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "兄弟的祭奠";
	elseif task:GetTaskSubmitNpc(10309) == npcId then
		if Task_Submit_00010309() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10309
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "兄弟的祭奠";
		elseif task:HasAcceptedTask(10309) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10309
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "兄弟的祭奠";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00010309_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士"..GetPlayerName(GetPlayer()).."，我已经看到了你的进步，这个世界上各种势力对我们的威胁越来越明显，战争恐怕也是一触即发，做好准备吧，如果我们再次面临天魔之战，天族需要你贡献你的力量。";
	action.m_ActionMsg = "我知道天族需要我。";
	return action;
end

function Task_00010309_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "又到了我们每年祭奠战争中死去的兄弟的时间了，可是我最近政务缠身，只能拜托你去了，顺便认识一下我的好兄弟明镜，他就在位于北部的瞿卢岭的法鼓山。带上这束香火，帮我为我的难兄神秀点上，寄托我对他的思念吧。";
	action.m_ActionMsg = "哦，是在法鼓山啊，为您的兄弟祭奠，我去了。";
	return action;
end

function Task_00010309_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "婆修又是派你自己来的？他怎么这么忙。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00010309_step_table = {
		[1] = Task_00010309_step_01,
		[2] = Task_00010309_step_02,
		[10] = Task_00010309_step_10,
		};

function Task_00010309_step(step)
	if Task_00010309_step_table[step] ~= nil then
		return Task_00010309_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00010309_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00010309() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15025, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(10309) then
		return false;
	end
	task:AddTaskStep(10309);
	return true;
end



--�ύ����
function Task_00010309_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15025,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(10309) then
		return false;
	end

	package:DelItemAll(15025,1);

	player:AddExp(1800);
	player:getCoin(3100);
	player:getTael(10);
	return true;
end

--��������
function Task_00010309_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15025,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(10309);
end
