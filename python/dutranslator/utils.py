def build_path( paths ):
    path = ""
    for p in paths:
        if not path.endswith( '/' ) and not path.endswith( '\\' ):
            path = path + '/'
        path = path + p
    if path.endswith('/') or path.endswith('\\'):
        path = path[0:-1]

    return path
