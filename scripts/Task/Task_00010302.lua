--����Ľ�������
function Task_Accept_00010302()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 18 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(10302) or task:HasCompletedTask(10302) or task:HasSubmitedTask(10302) then
		return false;
	end
	if not task:HasSubmitedTask(10301) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00010302()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 18 then
		return false;
	end
	if task:HasAcceptedTask(10302) or task:HasCompletedTask(10302) or task:HasSubmitedTask(10302) then
		return false;
	end
	if not task:HasSubmitedTask(10301) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00010302()
	if GetPlayer():GetTaskMgr():HasCompletedTask(10302) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00010302(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(10302) == npcId and Task_Accept_00010302 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 10302
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "会见王妃";
	elseif task:GetTaskSubmitNpc(10302) == npcId then
		if Task_Submit_00010302() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10302
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "会见王妃";
		elseif task:HasAcceptedTask(10302) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10302
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "会见王妃";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00010302_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "天族的勇士"..GetPlayerName(GetPlayer()).."您好，您是来自须弥山吗，我听说有一位魔族的公主现在住在那里。";
	action.m_ActionMsg = "魔族公主？你是说悦意王妃吗？";
	return action;
end

function Task_00010302_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "是的，您真是一位亲切的勇士，我不便说出我的身份来，我本来想跟悦意公主取得联系，可是我带来确认身份的密物被那边伏魔滩的浅水渔人抢走了，您能帮我抢回来，并交给悦意公主吗？公主一定会给你非常丰厚的回报的。";
	action.m_ActionMsg = "居然又是在我们天族地盘上的怪物，你等着我去解决他们。";
	return action;
end

function Task_00010302_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "（公主打开了包裹看到了里面的东西，皱了下眉头）你居然带来了这个？！不过还是要感谢你。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00010302_step_table = {
		[1] = Task_00010302_step_01,
		[2] = Task_00010302_step_02,
		[10] = Task_00010302_step_10,
		};

function Task_00010302_step(step)
	if Task_00010302_step_table[step] ~= nil then
		return Task_00010302_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00010302_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00010302() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(10302) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15021,1);
	if itemNum ~= 0 then
		if itemNum > 1 then
			itemNum = 1;
			package:SetItem(15021, itemNum, 1);
		end
		task:AddTaskStep2(10302, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00010302_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15021,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(10302) then
		return false;
	end

	package:DelItemAll(15021,1);

	player:AddExp(1800);
	player:getCoin(1700);
	return true;
end

--��������
function Task_00010302_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15021,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(10302);
end
