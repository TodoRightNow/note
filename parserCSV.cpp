#include <iostream>
#include <iterator>
#include <vector>
#include <string>
#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_file_iterator.hpp>

using namespace std;
using namespace boost::spirit;

template <typename iteratorT>

struct  LineInfo
{
    iteratorT lineIter;
    size_t lineNum;

    LineInfo(iteratorT beg):lineIter(beg),lineNum(1){}
};

template <typename iteratorT> 

struct NewLine 
{
    LineInfo<iteratorT>& lineInfo;

    NewLine(LineInfo<iteratorT>& info):lineInfo(info){}

    void operator()(iteratorT first,iteratorT last)const
    {
        lineInfo.lineIter=last;
        lineInfo.lineNum++;
    }
};


struct AddVal 
{
    vector<string>& values;
    AddVal(vector<string>& vec):values(vec){}


    template <class iteratorT> 
    void operator()(iteratorT first,iteratorT last)const
    {
        #ifdef  _MSC_VER
            std::string s;
            s.resize(distance(first,last));
            
            for(size_t i = 0; first!=last; i++)
            {
                s[i]=*first++;
            }
            
        #else 
              string s(first,last);
        #endif

        string::size_type pos=0;
        while(1)
        {
            pos=s.find("\"\"",pos);

            if(pos==string::npos)
            {
                break;
            }
            ++pos;
            s.erase(pos,1);
        }

        values.push_back(s);
    }
};

template <class iteratorT>


//解析某一格式封装成一个类，里面定义 语法规则，并注入自己定义的回调函数
struct CSVParser:public boost::spirit::classic::grammar<CSVParser<iteratorT> > 
{
    vector<string> &v;
    LineInfo<iteratorT>& lineInfo;
    

    CSVParser(vector<string>& vec,LineInfo<iteratorT>& info):v(vec),lineInfo(info){}

    template <typename ScannerT> 
    
    struct  definition
    {
        classic::rule<ScannerT> csv,val,quoted_val,naked_val;

        definition(const CSVParser<iteratorT> &self)
        {
            csv=val >>*(','>>val)>>(classic::eol_p[NewLine<iteratorT>(self.lineInfo)]|classic::end_p);

            val=*classic::blank_p>>classic::ch_p('\"')>>quoted_val[AddVal(self.v)]>>classic::ch_p('\"')
                 >>*classic::blank_p|naked_val[AddVal(self.v)];
            
            quoted_val=*(classic::eol_p[NewLine<iteratorT>(self.lineInfo)]|~classic::ch_p('"')|classic::str_p("\"\""));

            naked_val=*(~classic::ch_p(',')&~classic::ch_p('\"')&~classic::ch_p('\n'));
        }

        const classic::rule<ScannerT>& start() const {return csv;}
    };
    
    
};


template<typename iteratorT>
classic::parse_info<iteratorT> parse_csv(const iteratorT &first,const iteratorT& last,
vector<string>& vec,LineInfo<iteratorT>& info)
{
    CSVParser<iteratorT> csv(vec,info);
    return classic::parse(first,last,csv);
}

int  main(int argc, char const *argv[])
{
    if(argc!=2)
    {
        return 1;
    }

    typedef classic::file_iterator<char> iterator_t;
     //静态构造函数
    iterator_t begin(argv[1]);

    if(!begin)
    {
        cout<<"Unable to open the file:"<<argv[1]<<endl;
        return -1;
    }

    iterator_t first=begin;
    iterator_t last=first.make_end();

    LineInfo<iterator_t> line_info(begin);
    while(first!=last)
    {
        vector<string> v;

        classic::parse_info<iterator_t> info=parse_csv(first,last,v,line_info);

        if(!info.hit)
        {
            cout<<"error!line:"<<line_info.lineNum<<",Column:"
            <<distance(line_info.lineIter,info.stop)+1<<endl;
            break;
        }

        cout<<"parses OK:"<<endl;

    
    for(vector<string>::size_type i = 0; i < v.size(); i++)
    {
        cout<<i+1<<":"<<v[i]<<endl;

        cout<<"-------------------------\n";
        first=info.stop;
    }
    
    }
    return 0;
}
