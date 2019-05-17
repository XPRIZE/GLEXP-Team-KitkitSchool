/*
 * Copyright (C) 2013 Chungdahm Learning
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * Created on 2014.05.23
 */
package com.maq.xprize.xprize_gallery.utility;

import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ImageView;

/**
 * <pre>
 * Activity 종료시 해당 남아있는 View 에 관련한 resource 를 자동으로 해제 해주는 class
 * </pre>
 * 
 * @author Yong-Soo Hong
 * 
 */
public class RecycleUtils
{
	@SuppressWarnings("deprecation")
	public static void recycle(View view)
	{
		if (view == null)
		{
			return;
		}

		if (view.getBackground() != null)
		{
			view.getBackground().setCallback(null);
		}

		view.setBackgroundDrawable(null);

		if (view instanceof ViewGroup)
		{
			for (int i = 0; i < ((ViewGroup) view).getChildCount(); i++)
			{
				recycle(((ViewGroup) view).getChildAt(i));
			}

			if ((view instanceof AdapterView) == false)
			{
				((ViewGroup) view).removeAllViews();
			}
		}

		if (view instanceof ImageView)
		{
			((ImageView) view).setImageDrawable(null);
		}

		view = null;
	}
}
