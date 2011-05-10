--����Ľ�������
function Task_Accept_00040405()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 11 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40405) or task:HasCompletedTask(40405) or task:HasSubmitedTask(40405) then
		return false;
	end
	if not task:HasSubmitedTask(40404) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00040405()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 11 then
		return false;
	end
	if task:HasAcceptedTask(40405) or task:HasCompletedTask(40405) or task:HasSubmitedTask(40405) then
		return false;
	end
	if not task:HasSubmitedTask(40404) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00040405()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40405) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00040405(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40405) == npcId and Task_Accept_00040405 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40405
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "谨慎准备";
	elseif task:GetTaskSubmitNpc(40405) == npcId then
		if Task_Submit_00040405() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40405
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "谨慎准备";
		elseif task:HasAcceptedTask(40405) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40405
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "谨慎准备";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00040405_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士，看到你我就能猜到，肯定又是我师父让你走上这条历练之路的，他很相信这件事情对人的思想和能力的试炼，我曾经也经历过。";
	action.m_ActionMsg = "哦原来是这样。";
	return action;
end

function Task_00040405_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "长生丹并不是给普通人准备的，一旦使用不慎，几乎可以让人走火入魔到无法预知的疯狂境地 ，但是凭你的实力，只要遵照我的指导，应该不会出现这样的问题，现在你准备好了吗？你真的不担心，如果失败了可能造成的可怕后果吗？";
	action.m_ActionMsg = "是的，我认为没有问题。";
	return action;
end

function Task_00040405_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "做好准备接受你生来最疯狂的历练吧。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00040405_step_table = {
		[1] = Task_00040405_step_01,
		[2] = Task_00040405_step_02,
		[10] = Task_00040405_step_10,
		};

function Task_00040405_step(step)
	if Task_00040405_step_table[step] ~= nil then
		return Task_00040405_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00040405_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040405() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(40405) then
		return false;
	end
	task:AddTaskStep(40405);
	return true;
end



--�ύ����
function Task_00040405_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(40405) then
		return false;
	end


	player:AddExp(1200);
	player:getCoin(1150);
	return true;
end

--��������
function Task_00040405_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(40405);
end
