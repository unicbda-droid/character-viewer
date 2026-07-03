import struct, json

with open(r'C:\Users\ionco\character\run_speed.glb', 'rb') as f:
    data = f.read()

magic = struct.unpack_from('I', data, 0)[0]
version = struct.unpack_from('I', data, 4)[0]
length = struct.unpack_from('I', data, 8)[0]
print(f"GLB: magic=0x{magic:08X} version={version} length={length}")

# Parse JSON chunk
json_len = struct.unpack_from('I', data, 12)[0]
json_type = struct.unpack_from('I', data, 16)[0]
print(f"JSON chunk: len={json_len} type=0x{json_type:08X}")
json_str = data[20:20+json_len].decode('utf-8')
scene = json.loads(json_str)

# Print scene structure
print(f"\nNodes ({len(scene.get('nodes',[]))}):")
for i, n in enumerate(scene.get('nodes',[])):
    print(f"  [{i}] name={n.get('name','?')} children={n.get('children',[])}")

print(f"\nSkins ({len(scene.get('skins',[]))}):")
for i, s in enumerate(scene.get('skins',[])):
    print(f"  Skin {i}: skeleton={s.get('skeleton','?')} joints={s.get('joints',[])}")
    print(f"    inverseBindMatrices accessor={s.get('inverseBindMatrices','?')}")

print(f"\nBufferViews ({len(scene.get('bufferViews',[]))}):")
for i, bv in enumerate(scene.get('bufferViews',[])):
    print(f"  [{i}] byteOffset={bv.get('byteOffset',0)} byteLength={bv.get('byteLength',0)} byteStride={bv.get('byteStride','none')}")

print(f"\nAccessors ({len(scene.get('accessors',[]))}):")
for i, a in enumerate(scene.get('accessors',[])):
    t = a.get('type','?')
    comp = a.get('componentType','?')
    cnt = a.get('count','?')
    bv = a.get('bufferView','?')
    stride = a.get('byteStride','?')  # may or may not exist
    off = a.get('byteOffset',0)
    print(f"  [{i}] type={t} comp=0x{comp:X} count={cnt} bufferView={bv} byteOffset={off} byteStride={stride}")

print(f"\nAnimations ({len(scene.get('animations',[]))}):")
for ai, anim in enumerate(scene.get('animations',[])):
    print(f"  Animation {ai}: name={anim.get('name','?')}")
    for ci, ch in enumerate(anim.get('channels',[])):
        target = ch.get('target',{})
        print(f"    Channel {ci}: path={target.get('path','?')} node={target.get('node','?')} sampler={ch.get('sampler','?')}")

print(f"\nMeshes ({len(scene.get('meshes',[]))}):")
for mi, m in enumerate(scene.get('meshes',[])):
    print(f"  Mesh {mi}: name={m.get('name','?')}")
    for pi, p in enumerate(m.get('primitives',[])):
        attrs = p.get('attributes',{})
        print(f"    Primitive {pi}:")
        for name, acc in attrs.items():
            print(f"      {name} -> accessor {acc}")
        idx = p.get('indices','?')
        print(f"      INDICES -> accessor {idx}")

# Print first few bones and their parents
print("\nNode hierarchy with bones:")
joints = scene['skins'][0]['joints']
for ji, j in enumerate(joints):
    node = scene['nodes'][j]
    parent = node.get('parent','?')
    print(f"  Joint[{ji}] = node[{j}] name={node.get('name','?')} parent_idx={parent}")
