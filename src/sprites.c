#include <display.h>
#include <sprites.h>
#include <sys/reent.h>

uint32_t spr_frm = 0;

#define SPRITE_ADD(i, x) s_sprites[i] = SPRITE_FUNC(x, create)();

static void merge(sprite* arr[], int l, int mid, int r)
{
        int i, j, k;

        int n1 = mid - l + 1;
        int n2 = r - mid;

        sprite* larr[n1], *rarr[n2];

        for (i = 0; i < n1; i++)
                larr[i] = arr[l + i];
        for (j = 0; j < n2; j++)
                rarr[j] = arr[mid + 1 + j];

        k = l;
        i = 0;
        j = 0;

        while (i < n1 && j < n2)
        {
                if (larr[i]->zindex <= rarr[j]->zindex)
                {
                        arr[k] = larr[i];

                        i++;
                }
                else
                {
                        arr[k] = rarr[j];

                        j++;
                }

                k++;
        }

        while (i < n1)
        {
                arr[k] = larr[i];

                i++;
                k++;
        }

        while (j < n2)
        {
                arr[k] = rarr[j];

                j++;
                k++;
        }
}

static void merge_sort(sprite* arr[], int l, int r)
{
    if (l < r)
    {
        int mid = l + (r - l) / 2;

        merge_sort(arr, l, mid);
        merge_sort(arr, mid + 1, r);

        merge(arr, l, mid, r);
    }
}

/* Sprite state */
static sprite* s_sprites[NUM_SPRITES];

void init_sprites()
{
    SPRITE_ADD(0, char);

    for (int i = 0; i < NUM_SPRITES; i++)
    {
        sprite* spr = s_sprites[i];

        spr->init(spr);
    }

    merge_sort(s_sprites, 0, NUM_SPRITES-1);

    sprites_step(); // Do initial render.
}

void sprites_step()
{
    for (int i = 0; i < NUM_SPRITES; i++) 
    {
        sprite* spr = s_sprites[i];
        if (!spr->active) continue;

        s_sprites[i]->step(s_sprites[i]);
    }
}

void render_sprite(sprite *const self)
{
    if (self->oldx != -1 && self->oldy != -1) 
    {
        if (self->oldx == self->x && self->oldy == self->y) goto img;

        fill_rect(self->oldx, self->oldy, self->width, self->height, 0x0000);
    }

img: put_image(self->x, self->y, self->width, self->height, self->img_data, self->horientation, self->vorientation);
}

static bool point_is_intersecting(sprite* const self, uint16_t px, uint16_t py)
{
    uint16_t x1 = self->x, 
             y1 = self->y, 
             x2 = self->x + self->width, 
             y2 = self->y + self->height;

    return px >= x1 && px <= x2 && py >= y1 && py <= y2;
}

bool is_intersecting(sprite* const self, sprite* const other)
{
    return point_is_intersecting(self, other->x, other->y) || // top left
           point_is_intersecting(self, other->x + other->width, other->y) || // top right
           point_is_intersecting(self, other->x, other->y + other->height) || // bottom left
           point_is_intersecting(self, other->x + other->width, other->y + other->height); // bottom right
}
