--����Ľ�������
function Task_Accept_00010403()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 23 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(10403) or task:HasCompletedTask(10403) or task:HasSubmitedTask(10403) then
		return false;
	end
	if not task:HasSubmitedTask(10402) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00010403()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 23 then
		return false;
	end
	if task:HasAcceptedTask(10403) or task:HasCompletedTask(10403) or task:HasSubmitedTask(10403) then
		return false;
	end
	if not task:HasSubmitedTask(10402) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00010403()
	if GetPlayer():GetTaskMgr():HasCompletedTask(10403) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00010403(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(10403) == npcId and Task_Accept_00010403 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 10403
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "战争的缘由";
	elseif task:GetTaskSubmitNpc(10403) == npcId then
		if Task_Submit_00010403() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10403
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "战争的缘由";
		elseif task:HasAcceptedTask(10403) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10403
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "战争的缘由";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00010403_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "那时帝释天派了一个乐师带了礼物来到地界找阿修罗提亲，说想要将我娶回天庭，可当时刚好我的父亲毗摩质多罗在和阿修罗商议大事，我的父亲是阿修罗最敬重的火神，他们对于帝释天这种狂妄的行为非常生气，于是就带领魔族打了过来。";
	action.m_ActionMsg = "可是你为什么还是嫁到天庭来了呢？";
	return action;
end

function Task_00010403_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "那场战争实在是打了太久了，你们天族拥有甘露，可以帮助恢复延长战斗力，这一点上我们魔族不占有优势，而且战争中的死伤太多了，我不愿意因为我而造成这样的后果，于是我希望以我为条件，来结束战争。现在你想知道的我都告诉你了，回去找明镜吧。";
	action.m_ActionMsg = "那当然，我们天族绝对是最强大的。";
	return action;
end

function Task_00010403_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "悦意王妃已经将你想知道的告诉你了吧。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00010403_step_table = {
		[1] = Task_00010403_step_01,
		[2] = Task_00010403_step_02,
		[10] = Task_00010403_step_10,
		};

function Task_00010403_step(step)
	if Task_00010403_step_table[step] ~= nil then
		return Task_00010403_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00010403_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00010403() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(10403) then
		return false;
	end
	task:AddTaskStep(10403);
	return true;
end



--�ύ����
function Task_00010403_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(10403) then
		return false;
	end


	player:AddExp(2200);
	player:getCoin(3900);
	player:getTael(10);
	return true;
end

--��������
function Task_00010403_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(10403);
end
