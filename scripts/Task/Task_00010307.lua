--����Ľ�������
function Task_Accept_00010307()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 18 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(10307) or task:HasCompletedTask(10307) or task:HasSubmitedTask(10307) then
		return false;
	end
	if not task:HasSubmitedTask(10306) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00010307()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 18 then
		return false;
	end
	if task:HasAcceptedTask(10307) or task:HasCompletedTask(10307) or task:HasSubmitedTask(10307) then
		return false;
	end
	if not task:HasSubmitedTask(10306) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00010307()
	if GetPlayer():GetTaskMgr():HasCompletedTask(10307) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00010307(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(10307) == npcId and Task_Accept_00010307 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 10307
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "清除阻碍";
	elseif task:GetTaskSubmitNpc(10307) == npcId then
		if Task_Submit_00010307() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10307
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "清除阻碍";
		elseif task:HasAcceptedTask(10307) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10307
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "清除阻碍";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00010307_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "太好了，你为我带来了悦意公主的信，这真是让我感激不尽，现在我的任务完成了。";
	action.m_ActionMsg = "不用客气。";
	return action;
end

function Task_00010307_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "不过尊敬的勇士，你注意到我受伤了吗，附近的瓦萨部落的那些食人族，实在是太恐怖了，昨天我被他们抓到了，几乎九死一生，还好我侥幸逃脱。现在我必须带伤离开这里，可以请求您去解决掉他们吗？这样我应该可以活着离开了，您也可以回到婆修大人那儿去了。";
	action.m_ActionMsg = "没问题，我这就去帮你解决他们。";
	return action;
end

function Task_00010307_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "哦勇士你又回来找我了，看来那个身份神秘的人已经没事了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00010307_step_table = {
		[1] = Task_00010307_step_01,
		[2] = Task_00010307_step_02,
		[10] = Task_00010307_step_10,
		};

function Task_00010307_step(step)
	if Task_00010307_step_table[step] ~= nil then
		return Task_00010307_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00010307_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00010307() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(10307) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00010307_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8904,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(10307) then
		return false;
	end

	if IsEquipTypeId(8904) then
		for k = 1, 1 do
			package:AddEquip(8904, 1);
		end
	else 
		package:AddItem(8904,1,1);
	end

	player:AddExp(1800);
	player:getCoin(2700);
	player:getTael(10);
	return true;
end

--��������
function Task_00010307_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(10307);
end
