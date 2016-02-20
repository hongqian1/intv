// Trie impl
import java.util.HashMap;

class TrieNode {
	 boolean isString;
	 HashMap<Character, TrieNode> subtree;
	 public TrieNode() {
		// TODO Auto-generated constructor stub
		 isString = false;
		 subtree = new HashMap<Character, TrieNode>();
	 }
};


class TrieTree{
	TrieNode root ;
	public TrieTree(TrieNode TrieNode) {
		root = TrieNode;
	}
	public void insert(String s) {
		TrieNode now = root;
		for (int i = 0; i < s.length(); i++) {
			if (!now.subtree.containsKey(s.charAt(i))) {
				now.subtree.put(s.charAt(i), new TrieNode());
			}
			now  =  now.subtree.get(s.charAt(i));
		}
		now.isString  = true;
	}
	public boolean find(String s){
		TrieNode now = root;
		for (int i = 0; i < s.length(); i++) {
			if (!now.subtree.containsKey(s.charAt(i))) {
				return false;
			}
			now  =  now.subtree.get(s.charAt(i));
		}
		return now.isString ;
	}
};

public class Main {
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		int ty;
		TrieTree tree =  new TrieTree(new TrieNode());
		
		tree.insert("aab");
		tree.insert("aaba");
		tree.insert("aabb");
		tree.insert("aabbb");
		tree.insert("aabcc");
		
		System.out.println(tree.find("aab"));

		System.out.println(tree.find("aabc"));

		System.out.println(tree.find("aabc"));
	}
}

// max tree:
/**
 * Definition of TreeNode: public class TreeNode { public int val; public
 * TreeNode left, right; public TreeNode(int val) { this.val = val; this.left =
 * this.right = null; } }
 */
public class Solution {

	public static TreeNode maxTree(int[] A) {
		// write your code here

		Stack<TreeNode> stack = new Stack<TreeNode>();

		TreeNode root = null;
		for (int i = 0; i <= A.length; i++) {

			TreeNode right = i == A.length ? new TreeNode(Integer.MAX_VALUE)
					: new TreeNode(A[i]);

			while (!stack.isEmpty()) {
				if (right.val > stack.peek().val) {
					TreeNode nodeNow = stack.pop();

					if (stack.isEmpty()) {
						right.left = nodeNow;

					} else {
						TreeNode left = stack.peek();
						if (left.val > right.val) {
							right.left = nodeNow;
						} else {
							left.right = nodeNow;
						}
					}
				} else {
					break;
				}
			}
			stack.push(right);
		}
		return stack.peek().left;
	}
}
