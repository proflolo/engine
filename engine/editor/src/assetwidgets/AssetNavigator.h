#pragma once

namespace editor
{
	class AssetNavigator: public juce::Component, public juce::FileBrowserListener
	{
	public:
		AssetNavigator(const ProjectContext& i_projectContext);
		~AssetNavigator();

		void resized() override;

	private:
		juce::WildcardFileFilter imagesWildcardFilter; // { "*.jpeg;*.jpg;*.png;*.gif", "*", "Image File Filter" };
		juce::TimeSliceThread directoryThread{ "Image File Scanner Thread" };
		juce::DirectoryContentsList imageList{ &imagesWildcardFilter, directoryThread };
		juce::FileTreeComponent fileTree{ imageList };

		void selectionChanged() override
		{
			// we're only really interested in when the selection changes, regardless of if it was
			// clicked or not so we'll only override this method
			auto selectedFile = fileTree.getSelectedFile();

			if (selectedFile.existsAsFile())
			{
				
			}

			// the image cache is a handy way to load images from files or directly from memory and
			// will keep them hanging around for a few seconds in case they are requested elsewhere
		}

		void fileClicked(const juce::File&, const juce::MouseEvent&) override {}
		void fileDoubleClicked(const juce::File&)              override {}
		void browserRootChanged(const juce::File&)             override {}

		void UpdateLayout();
	};
}