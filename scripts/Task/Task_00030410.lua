--����Ľ�������
function Task_Accept_00030410()
	local player = GetPlayer();
	if player:GetLev() < 39 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30410) or task:HasCompletedTask(30410) or task:HasSubmitedTask(30410) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30409) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30409) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030410()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 39 then
		return false;
	end
	if task:HasAcceptedTask(30410) or task:HasCompletedTask(30410) or task:HasSubmitedTask(30410) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30409) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30409) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030410()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30410) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030410(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30410) == npcId and Task_Accept_00030410 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30410
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "隐士的心事";
	elseif task:GetTaskSubmitNpc(30410) == npcId then
		if Task_Submit_00030410() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30410
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "隐士的心事";
		elseif task:HasAcceptedTask(30410) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30410
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "隐士的心事";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030410_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "果然没有辜负城主的信任和我帮忙铸造的武器啊，这么快就打败那些狼妖回来了，看来我们都没有看错你，年轻的勇士，相信你在山海大陆一定会有一番作为的。";
	action.m_ActionMsg = "我会继续努力的。";
	return action;
end

function Task_00030410_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "其实不瞒你说，我之所以隐居在这森林里的茅屋，是有隐情的，为你铸造武器，我也是有私心的，我想你欠我一个人情，如果你愿意，希望你能去山河城的河川旅店见那里的老板娘随形一面，并把这串宝石项链交给她。";
	action.m_ActionMsg = "当然没问题，我现在就过去。";
	return action;
end

function Task_00030410_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我们的小旅店有勇士的光临，真是蓬荜生辉。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030410_step_table = {
		[1] = Task_00030410_step_01,
		[2] = Task_00030410_step_02,
		[10] = Task_00030410_step_10,
		};

function Task_00030410_step(step)
	if Task_00030410_step_table[step] ~= nil then
		return Task_00030410_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030410_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030410() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15040, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(30410) then
		return false;
	end
	task:AddTaskStep(30410);
	return true;
end



--�ύ����
function Task_00030410_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15040,1) < 1 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(8912,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(30410) then
		return false;
	end

	if IsEquipTypeId(8912) then
		for k = 1, 1 do
			package:AddEquip(8912, 1);
		end
	else 
		package:AddItem(8912,1,1);
	end
	package:DelItemAll(15040,1);

	player:AddExp(8000);
	player:getCoin(12000);
	player:getTael(20);
	return true;
end

--��������
function Task_00030410_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15040,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30410);
end
